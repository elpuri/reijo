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
