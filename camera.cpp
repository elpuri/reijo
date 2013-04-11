#include "camera.h"
#include <QVector3D>
#include <QMatrix4x4>
#include <QMetaProperty>
#include <QDebug>
#include <math.h>

#include "ray.h"

Camera::Camera(QObject *parent) :
    SceneNode(parent), m_up(0.0, 1.0, 0.0)
{
}


QVector3D Camera::up()
{
    return m_up;
}

// Boohoo I'm so upset
void Camera::upSetter(QVector3D up)
{
    m_up = up;
    calcTransformation();
}

QVariant Camera::lookAt()
{
    return m_lookAt;
}

void Camera::lookAtSetter(QVariant target)
{
    // If target is a QVector3D, use it as is
    // If target is a QObject*, look for the "position" property and connect to listen to changes if possible
    m_lookAt = target;
    qDebug() << "Look at target type" << target.typeName();
    if (target.type() == QMetaType::QVector3D) {
        disconnect(this, SLOT(onTargetObjectPositionChanged()));   // in case lookAt was previously a QObject*
        m_target = target.value<QVector3D>();
        qDebug() << "look at vec:" << m_target;
    } else if (target.type() == QMetaType::QObjectStar) {
        qDebug() << "Camera: lookat target is a QObject*";
        m_targetObject = target.value<QObject*>();

        const QMetaObject* mo = m_targetObject->metaObject();
        int propCount = mo->propertyCount();
        bool found = false;
        for (int i=0; i < propCount; i++) {
            QMetaProperty mp = mo->property(i);
            qDebug() << mp.name();
            if (QString(mp.name()).compare("position") == 0) {
                found = true;
                QVariant positionVariant = mp.read(m_targetObject);
                if (positionVariant.type() != QMetaType::QVector3D)
                    qDebug() << "Position property of lookAt object isn't of type QVector3D";
                else
                    m_target = positionVariant.value<QVector3D>();
    //            if (mp.hasNotifySignal())
     //               connect(m_targetObject, mo->mmp.notifySignalIndex(), this, SLOT(onTargetObjectPositionChanged()));
                break;
            }
        }
        if (!found)
            qDebug() << "Object assigned to lookAt doesn't have a position property";

    } else
        qDebug() << "Why did you try to assign" << target.typeName() << "to lookAt?";
    calcTransformation();

}

void Camera::generateRay(Ray &r, float x, float y, float aspectRatio)
{
    QVector4D direction;
    double fovRad = m_fov / 360 * 2.0 * M_PI;
    float xSpan = tan(fovRad) * aspectRatio;
    float ySpan = tan(fovRad);
    direction.setX(-xSpan / 2 + xSpan * x);
    direction.setY(ySpan / 2 - ySpan * y);
    direction.setZ(1.0);
    direction.setW(0.0);
    direction = m_transformation * direction;
    QVector4D position = m_transformation * QVector4D(0.0, 0.0, 0.0, 1.0);
    r.set(position, direction);
}

void Camera::onTargetObjectPositionChanged()
{
    qDebug() << Q_FUNC_INFO;
}

void Camera::calcTransformation()
{
    m_transformation.setToIdentity();

    // Calc a vector that points right from the gaze and up vectors
    QVector3D up = m_up.normalized();
    QVector3D gaze = (m_target - m_position).normalized();
    QVector3D right = QVector3D::crossProduct(-gaze, up);

    // The ONB defined by right, up and gaze sets up the rotation and then we use the
    // camera position for translation
    m_transformation.setColumn(0, right.toVector4D());
    m_transformation.setColumn(1, up.toVector4D());
    m_transformation.setColumn(2, gaze.toVector4D());
    m_transformation.setColumn(3, QVector4D(m_position, 1.0));
}
