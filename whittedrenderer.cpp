#include <QElapsedTimer>

#include "whittedrenderer.h"
#include "scene.h"
#include "camera.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "material.h"
#include "mathutils.h"

WhittedRenderer::WhittedRenderer(QObject *parent) :
    Renderer(parent),
    m_buffer(nullptr),
    m_maxRecursionDepth(5),
    m_coloredShadows(false)
{
}


void WhittedRenderer::render(Scene* scene)
{
    qDebug() << Q_FUNC_INFO;


    Camera* camera = scene->activeCamera();
    QList<Shape*> shapes = scene->gatherShapes();
    QList<Light*> lights = scene->gatherLights();

    qDebug() << "Rendering" << shapes.count() << "shapes with" << lights.count() << "lights";

    if (camera->fov() <=0) {
        qCritical() << "Active camera has invalid fov";
        return;
    }

    QElapsedTimer elapsed;
    elapsed.start();

    if (m_buffer)
        delete m_buffer;

    m_buffer = new float[m_renderedWidth * m_renderedHeight * 3];
    float* buffer = m_buffer;
    float aspect = (float) m_renderedWidth / m_renderedHeight;
    for (int y=0; y < m_renderedHeight; y++) {
        for (int x=0; x < m_renderedWidth; x++) {
            Ray primaryRay;
            camera->generateRay(primaryRay, (float)(x + 1) / m_renderedWidth, (float)(y + 1) / m_renderedHeight, aspect);
            QVector3D shaded = trace(primaryRay, 0, shapes, lights, QVector4D(camera->position(), 1.0));
            *buffer++ = shaded.x(); *buffer++ = shaded.y(); *buffer++ = shaded.z();
        }
    }

    qDebug() << "Render complete in:" << elapsed.elapsed() << "ms";
    emit frameComplete();
}

QVector3D WhittedRenderer::trace(const Ray& primaryRay, int depth, const QList<Shape*>& shapes, const QList<Light*>& lights,
                                 const QVector4D& cameraPosition)
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
        QVector4D viewVector = cameraPosition - hitPoint;
        foreach(Light* light, lights) {

            QVector3D emittance;
            QVector4D lightVector;
            light->sample(lightVector, emittance);
            lightVector = lightVector - hitPoint;
            QVector4D unnormalizedLightVector = lightVector;
            lightVector.normalize();

            Ray shadowRay(hitPoint, lightVector);
            bool inShadow = false;
            double t;
            foreach(Shape* shape, shapes) {
                if (shape != closestShape && shape->castsShadows()) {
                    bool intersects = shape->intersect(shadowRay, t);
                    bool furtherThanLight = unnormalizedLightVector.lengthSquared() <  t * t;
                    inShadow = intersects && t > 0.0 && !furtherThanLight;
                    if (inShadow)
                        break;
                }
            }

            if (!inShadow) {
                // Diffuse
                float dot = fmax(0.0f, QVector4D::dotProduct(lightVector, normalVector)) * material->diffuseReflectivity();
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
                shaded += material->reflectivity() * trace(reflectedRay, depth + 1, shapes, lights, cameraPosition);
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
