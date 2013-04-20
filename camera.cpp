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

QVector3D Camera::targetObjectPosition(QObject *o) const
{
    QVector3D targetPosition;
    const QMetaObject* mo = o->metaObject();
    int propCount = mo->propertyCount();
    bool found = false;
    for (int i=0; i < propCount; i++) {
        QMetaProperty mp = mo->property(i);
        if (QString(mp.name()).compare("position") == 0) {
            found = true;
            QVariant positionVariant = mp.read(o);
            if (positionVariant.type() != QMetaType::QVector3D)
                qDebug() << "Position property of lookAt object isn't of type QVector3D";
            else
                targetPosition = positionVariant.value<QVector3D>();
            if (mp.hasNotifySignal()) {
                // Isn't there a way to generate a signature that connect accepts?
                connect(m_targetObject, "2" + mp.notifySignal().methodSignature(), this, SLOT(onTargetObjectPositionChanged()));
            }
            break;
        }
    }
    if (!found)
        qDebug() << "Object assigned to lookAt doesn't have a position property";
    return targetPosition;
}

void Camera::lookAtSetter(QVariant target)
{
    // If target is a QVector3D, use it as is
    // If target is a QObject*, look for the "position" property and connect to listen to changes if possible
    m_lookAt = target;
    if (target.type() == QMetaType::QVector3D) {
        disconnect(this, SLOT(onTargetObjectPositionChanged()));   // in case lookAt was previously a QObject*
        m_target = target.value<QVector3D>();
    } else if (target.type() == QMetaType::QObjectStar) {
        m_targetObject = target.value<QObject*>();
        m_target = targetObjectPosition(m_targetObject);

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
    m_target = targetObjectPosition(m_targetObject);
    calcTransformation();
}

void Camera::calcTransformation()
{
    m_transformation.setToIdentity();

    // Calc a vector that points right from the gaze and up vectors
    QVector3D up = m_up.normalized();
    QVector3D gaze = (m_target - m_position).normalized();
    QVector3D right = QVector3D::crossProduct(-gaze, up);
    up = QVector3D::crossProduct(gaze, right);

    // The ONB defined by right, up and gaze sets up the rotation and then we use the
    // camera position for translation
    m_transformation.setColumn(0, right.toVector4D());
    m_transformation.setColumn(1, up.toVector4D());
    m_transformation.setColumn(2, gaze.toVector4D());
    m_transformation.setColumn(3, QVector4D(m_position, 1.0));
}
