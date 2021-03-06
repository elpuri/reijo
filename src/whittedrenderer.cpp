/*
-- Copyright (c) 2013, Juha Turunen (turunen@iki.fi)
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
-- ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QElapsedTimer>
#include <thread>

#include "whittedrenderer.h"
#include "scene.h"
#include "camera.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "material.h"
#include "mathutils.h"
#include "regulargrid.h"
#include "hdrviewer.h"

WhittedRenderer::WhittedRenderer(QObject *parent) :
    Renderer(parent),
    m_buffer(nullptr),
    m_maxRecursionDepth(5),
    m_coloredShadows(false),
    m_samplesPerPixel(4),
    m_antiAliasing(None),
    m_grid(nullptr)
{
    m_hwThreadCount = std::thread::hardware_concurrency();
    if (m_hwThreadCount == 0)
        m_hwThreadCount = 8;
    qDebug() << m_hwThreadCount;
    QThreadPool::globalInstance()->setMaxThreadCount(m_hwThreadCount);
}


void WhittedRenderer::render(Scene* scene)
{
    if (m_viewer)
        connect(m_viewer, &HdrViewer::clicked, this, &WhittedRenderer::onPicked);

    // TODO: this stuff should go to the base class
    qDebug() << Q_FUNC_INFO;

    qDebug() << "Rendering with" << m_hwThreadCount << "threads";

    m_activeCamera = scene->activeCamera();
    m_shapes.clear();
    m_shapes = scene->gatherShapes();
    m_lights.clear();
    m_lights = scene->gatherLights();


    qDebug() << "Rendering" << m_shapes.count() << "shapes with" << m_lights.count() << "lights";

    if (m_activeCamera->fov() <=0) {
        qCritical() << "Active camera has invalid fov";
        return;
    }

    if (m_buffer)
        delete m_buffer;

    m_buffer = new float[m_renderedWidth * m_renderedHeight * 3];

    emit renderingStarted();
    do {
        qDebug() << "Starting new frame" << scene->time();

        QElapsedTimer elapsed;
        elapsed.start();

        QElapsedTimer gridTime;
        gridTime.start();
        if (m_grid)
            delete m_grid;
        m_grid = new RegularGrid();
        m_grid->initialize(m_shapes);
        qDebug() << "Generated acceleration structure in" << gridTime.elapsed() << "ms";

        auto pool = QThreadPool::globalInstance();
        for (int i=0; i < m_hwThreadCount; i++) {
            WhittedRunnable* runnable = new WhittedRunnable(this, i, m_hwThreadCount);
            runnable->setAutoDelete(true);
            pool->start(runnable);
        }
        // TODO: write a thread pool that sends a signal when job is complete
        while (!pool->waitForDone(25)) {
            qApp->processEvents();
        }

        qDebug() << "Frame complete in:" << elapsed.elapsed() << "ms";
        emit frameComplete();
        qApp->processEvents();
    } while(scene->advanceFrame());

    emit renderingComplete();

}

void WhittedRenderer::renderScanline(int y)
{
    float* buffer = m_buffer + y * m_renderedWidth * 3;
    float aspect = (float) m_renderedWidth / m_renderedHeight;

    float xStep = 1.0 / (m_renderedWidth - 1);
    float yStep = 1.0 / (m_renderedHeight - 1);

    if (m_antiAliasing == None) {
        for (int x=0; x < m_renderedWidth; x++) {
            QVector3D shaded;
            Ray primaryRay;
            m_activeCamera->generateRay(primaryRay, x * xStep, y * yStep, aspect);
            shaded = trace(primaryRay, 0, m_shapes, m_lights);
            *buffer++ = shaded.x(); *buffer++ = shaded.y(); *buffer++ = shaded.z();
        }
    } else if (m_antiAliasing == JitteredSamples) {
        float sampleScaler = 1.0 / m_samplesPerPixel;
        for (int x=0; x < m_renderedWidth; x++) {
            QVector3D shaded;
            for (int i=0; i < m_samplesPerPixel; i++) {
                Ray primaryRay;
                float xJitter = i == 0 ? 0.0 : (MathUtils::randomf() * xStep);
                float yJitter = i == 0 ? 0.0 : (MathUtils::randomf() * yStep);
                float xSample = x * xStep + xJitter;
                float ySample = y * yStep + yJitter;
                m_activeCamera->generateRay(primaryRay, xSample, ySample, aspect);
                shaded += trace(primaryRay, 0, m_shapes, m_lights) * sampleScaler;
            }
            *buffer++ = shaded.x(); *buffer++ = shaded.y(); *buffer++ = shaded.z();
        }
    }
}

QVector3D WhittedRenderer::trace(const Ray& primaryRay, int depth, const QList<Shape*>& shapes, const QList<Light*>& lights)
{
    double minDist = std::numeric_limits<double>::max();
    Shape* closestShape = nullptr;

    QVector3D shaded;
    if (m_grid->intersect(primaryRay, closestShape, minDist)) {
        auto material = closestShape->material();
        shaded = material->ambientReflectivity() * m_ambientLightColor * material->colorVector();
        QColor color = material ? material->color() : QColor(40, 40, 40);
        QVector3D diffuseColor(color.redF(), color.greenF(), color.blueF());
        QVector4D hitPoint = primaryRay.along(minDist);
        QVector4D normalVector = closestShape->surfaceNormal(hitPoint, primaryRay);
        QVector4D viewVector = -primaryRay.direction();
        foreach(Light* light, lights) {

            QVector3D emittance;
            QVector4D lightVector;
            light->sample(lightVector, emittance);
            lightVector = lightVector - hitPoint;
            float lightVectorLengthSquared = lightVector.lengthSquared();
            float lightVectorLength = sqrt(lightVectorLengthSquared);
            lightVector.normalize();

            Ray shadowRay(hitPoint, lightVector);
            double t;
            Shape* blockingShape;
            bool shadowRayHit = m_grid->intersect(shadowRay, blockingShape, t);
            if (!shadowRayHit || (shadowRayHit && (lightVectorLengthSquared < t * t))) {
                // Diffuse
                float dot = fmax(0.0f, QVector4D::dotProduct(lightVector, normalVector)) * material->diffuseReflectivity();
                emittance *= 1 / (1 + 0.2 * lightVectorLength + 0.08 * lightVectorLengthSquared);
                shaded += dot * QVector3D(emittance.x() * diffuseColor.x(),
                                          emittance.y() * diffuseColor.y(),
                                          emittance.z() * diffuseColor.z());

                // Specular
                if (material->specularReflectivity() > 0.0) {
                    QVector4D reflectedLightVector = MathUtils::reflect(-lightVector, normalVector); // lightVector and normalVector are already normalized
                    float dot = QVector4D::dotProduct(reflectedLightVector, viewVector);
                    if (dot > 0.0)
                        shaded += material->specularReflectivity() * pow(dot, material->shininess()) * emittance;
                }
            }
        }

        if (depth < m_maxRecursionDepth) {
            // Spawn reflected ray
            if (material && material->reflectivity() > 0.0) {
                QVector4D reflectionVector = MathUtils::reflect(-viewVector, normalVector);
                Ray reflectedRay(hitPoint, reflectionVector);
                shaded += material->reflectivity() * trace(reflectedRay, depth + 1, shapes, lights);
            }

            if (material && material->transmittance() > 0.0) {
                bool inside = QVector4D::dotProduct(viewVector, normalVector) < 0;
                QVector4D refractionVector;
                bool totalReflection;

                if (!inside)    // outside
                    totalReflection =  MathUtils::refract(primaryRay.direction(), normalVector, 1.0, material->indexOfRefraction(), refractionVector, inside);
                else    // inside
                    totalReflection =  MathUtils::refract(primaryRay.direction(), -normalVector, material->indexOfRefraction(), 1.0, refractionVector, inside);

                if (!totalReflection) {
                    Ray transmittedRay(hitPoint, refractionVector);
                    QVector3D tc = trace(transmittedRay, depth + 1, shapes, lights);
                    shaded += material->transmittance() * tc * material->colorVector();
                } else {
                    Ray totalReflectionRay(hitPoint, MathUtils::reflect(primaryRay.direction(), -normalVector));
                    shaded += trace(totalReflectionRay, depth + 1, shapes, lights);
                }
            }
        }
    }
    return shaded;
}

void WhittedRenderer::copyRenderBuffer(float *&buffer)
{
    buffer = new float[m_renderedWidth * m_renderedHeight * 3];
    memcpy(buffer, m_buffer, m_renderedWidth * m_renderedHeight * sizeof(float) * 3);
}

void WhittedRenderer::classBegin()
{

}

void WhittedRenderer::componentComplete()
{

}


WhittedRenderer::WhittedRunnable::WhittedRunnable(WhittedRenderer *r, int interleave, int interleaveCount) :
    m_renderer(r),
    m_interleave(interleave),
    m_interleaveCount(interleaveCount)
{

}

void WhittedRenderer::WhittedRunnable::run()
{
    int line = m_interleave;
    while (line < m_renderer->m_renderedHeight) {
        m_renderer->renderScanline(line);
        line += m_interleaveCount;
    }
}

// For debugging purposes
void WhittedRenderer::onPicked(const int x, const int y)
{
    /*
    float aspect = (float) m_renderedWidth / m_renderedHeight;
    float xStep = 1.0 / (m_renderedWidth - 1);
    float yStep = 1.0 / (m_renderedHeight - 1);

    Ray primaryRay;
    m_activeCamera->generateRay(primaryRay, x * xStep, y * yStep, aspect);
    Shape* hitShape = nullptr;
    double t;
*/

}
