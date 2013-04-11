#ifndef WHITTEDRENDERER_H
#define WHITTEDRENDERER_H

#include "proputils.h"
#include "renderer.h"

#include <QtQml>
#include <QString>
#include <QVector3D>

class Ray;
class Shape;
class Light;

class WhittedRenderer : public Renderer,
                        public QQmlParserStatus
{
    Q_OBJECT

    SIMPLE_PROPERTY(QVector3D, ambientLightColor)
    SIMPLE_PROPERTY(int, maxRecursionDepth)
    SIMPLE_PROPERTY(bool, coloredShadows)

public:
    explicit WhittedRenderer(QObject *parent = 0);
    QVector3D trace(const Ray& r, int depth, const QList<Shape*>& shapes, const QList<Light*>& lights,
                    const QVector4D& cameraPosition);

public:
    void render(Scene* scene);
    void copyRenderBuffer(float *&buffer);

protected:
    void classBegin();
    void componentComplete();



private:
    float* m_buffer;
};

#endif // WHITTEDRENDERER_H
