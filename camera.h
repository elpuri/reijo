#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVariant>
#include <QVector3D>
#include <QMatrix4x4>

#include "proputils.h"
#include "scenenode.h"

class Ray;

class Camera : public SceneNode
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY(QVector3D, position)
    SIMPLE_NOTIFYING_PROPERTY(float, fov)
    SIMPLE_NOTIFYING_PROPERTY_DECL(QVariant, lookAt)
    SIMPLE_NOTIFYING_PROPERTY_DECL(QVector3D, up)

public:
    explicit Camera(QObject *parent = 0);

    // x = [0..1], y = [0..1]
    void generateRay(Ray&, float x, float y, float aspect);

private:
    void calcTransformation();

private slots:
    void onTargetObjectPositionChanged();


private:
    QVariant m_lookAt;
    QVector3D m_up;
    QVector3D m_target;
    QObject* m_targetObject;
    bool m_targetsObject;
    QMatrix4x4 m_transformation;


};

#endif // CAMERA_H
