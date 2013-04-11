#ifndef SHAPE_H
#define SHAPE_H

#include <QtQml>
#include <QMatrix4x4>
#include <QVector4D>

#include "scenenode.h"
#include "proputils.h"
#include "ray.h"

class Material;

class Shape : public SceneNode,
              public QQmlParserStatus
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY(QVector3D, position)
    SIMPLE_NOTIFYING_PROPERTY(QVector3D, rotation)
    SIMPLE_NOTIFYING_PROPERTY(QVector3D, scale)
    SIMPLE_PROPERTY(Material*, material)
    SIMPLE_NOTIFYING_PROPERTY(bool, castsShadows)

public:
    explicit Shape(QObject* parent = 0);

    virtual bool intersect(const Ray& ray, double& t) { return false; }
    virtual QVector4D surfaceNormal(const QVector4D& p) { return QVector4D(); }

    void classBegin();
    void componentComplete();
    void applyParentMatrix(const QMatrix4x4 m);
    const QMatrix4x4 compositeTransformation() const { return m_objectToWorld; }
    QString dumpProperties();

protected:
    QMatrix4x4 m_transformation;
    QMatrix4x4 m_objectToWorld;
    QMatrix4x4 m_worldToObject;
};

#endif // SHAPE_H
