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
    QVector3D targetObjectPosition(QObject*) const;

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
