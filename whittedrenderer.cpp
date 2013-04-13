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
#include "whittedrunnable.h"

WhittedRenderer::WhittedRenderer(QObject *parent) :
    Renderer(parent),
    m_buffer(nullptr),
    m_maxRecursionDepth(5),
    m_coloredShadows(false)
{
    m_hwThreadCount = std::thread::hardware_concurrency();
    if (m_hwThreadCount == 0)
        m_hwThreadCount = 8;
}


void WhittedRenderer::render(Scene* scene)
{
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

    QElapsedTimer elapsed;
    elapsed.start();

    if (m_buffer)
        delete m_buffer;

    m_buffer = new float[m_renderedWidth * m_renderedHeight * 3];

    auto pool = QThreadPool::globalInstance();
    for (int i=0; i < m_hwThreadCount; i++) {
        WhittedRunnable* runnable = new WhittedRunnable(this, i, m_hwThreadCount);
        runnable->setAutoDelete(true);
        pool->start(runnable);
    }

    pool->waitForDone();

    qDebug() << "Render complete in:" << elapsed.elapsed() << "ms";
    emit frameComplete();
}

void WhittedRenderer::renderScanline(int y)
{
    float* buffer = m_buffer + y * m_renderedWidth * 3;
    float aspect = (float) m_renderedWidth / m_renderedHeight;

    for (int x=0; x < m_renderedWidth; x++) {
        Ray primaryRay;
        m_activeCamera->generateRay(primaryRay, (float)(x + 1) / m_renderedWidth, (float)(y + 1) / m_renderedHeight, aspect);
        QVector3D shaded = trace(primaryRay, 0, m_shapes, m_lights);
        *buffer++ = shaded.x(); *buffer++ = shaded.y(); *buffer++ = shaded.z();
    }
}

QVector3D WhittedRenderer::trace(const Ray& primaryRay, int depth, const QList<Shape*>& shapes, const QList<Light*>& lights)
{
    double minDist = INFINITY;
    Shape* closestShape = nullptr;
    foreach(Shape* s, shapes) {
        double hitDist;
        if (s->intersect(primaryRay, hitDist)) {
            if (hitDist < minDist) {
                minDist = hitDist;
                closestShape = s;
            }
        }
    }

    QVector3D shaded;
    if (closestShape) {

        auto material = closestShape->material();
        shaded = material->ambientReflectivity() * m_ambientLightColor * material->colorVector();
        QColor color = material ? material->color() : QColor(40, 40, 40);
        QVector3D diffuseColor(color.redF(), color.greenF(), color.blueF());
        QVector4D hitPoint = primaryRay.along(minDist);
        QVector4D normalVector = closestShape->surfaceNormal(hitPoint);
        QVector4D viewVector = primaryRay.origin() - hitPoint;
        foreach(Light* light, lights) {

            QVector3D emittance;
            QVector4D lightVector;
            light->sample(lightVector, emittance);
            lightVector = lightVector - hitPoint;
            float lightVectorLengthSquared = lightVector.lengthSquared();
            float lightVectorLength = sqrt(lightVectorLengthSquared);
            lightVector.normalize();

            Ray shadowRay(hitPoint, lightVector);
            bool inShadow = false;
            double t;
            foreach(Shape* shape, shapes) {
                if (shape != closestShape && shape->castsShadows()) {
                    bool intersects = shape->intersect(shadowRay, t);
                    bool furtherThanLight = lightVectorLengthSquared <  t * t;
                    inShadow = intersects && t > 0.0 && !furtherThanLight;
                    if (inShadow)
                        break;
                }
            }

            if (!inShadow) {
                // Diffuse
                float dot = fmax(0.0f, QVector4D::dotProduct(lightVector, normalVector)) * material->diffuseReflectivity();
                emittance *= 1 / (1 + 0.2 * lightVectorLength + 0.08 * lightVectorLengthSquared);
                shaded += dot * QVector3D(emittance.x() * diffuseColor.x(),
                                          emittance.y() * diffuseColor.y(),
                                          emittance.z() * diffuseColor.z());

                // Specular
                if (material->specularReflectivity() > 0.0) {
                    QVector4D reflectedLightVector = MathUtils::reflect(-lightVector, normalVector); // lightVector and normalVector are already normalized
                    float dot = QVector4D::dotProduct(reflectedLightVector, viewVector.normalized());
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

            }
            // TODO: spawn transmitted ray
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
