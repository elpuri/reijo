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

#ifndef SPHERE_H
#define SPHERE_H

#include "proputils.h"
#include "shape.h"
#include <QDebug>

class Sphere : public Shape
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY(qreal, radius)

public:
    explicit Sphere(QObject* parent = 0);

    bool intersect(const Ray &ray, double &t);
    QVector4D surfaceNormal(const QVector4D &p, const Ray& ray);
    bool hasBoundingBox() const { return true; }
    BoundingBox objectSpaceBoundingBox() const {
        return BoundingBox(QVector4D(-m_radius, -m_radius, -m_radius, 1.0),
                           QVector4D(m_radius, m_radius, m_radius, 1.0));
    }
    BoundingBox worldSpaceBoundingBox() const;

protected:
    QString dumpProperties();
};

#endif // SPHERE_H
