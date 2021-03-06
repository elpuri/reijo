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

#include "plane.h"
#include "mathutils.h"

#include <QDebug>

Plane::Plane(QObject *parent) :
    Shape(parent)
{
}


QString Plane::dumpProperties()
{
    QString props = "[point: %1, %2, %3  normal: %4, %5, %6]";
    props = props.arg(m_point.x()).arg(m_point.y()).arg(m_point.z()).arg(m_normal.x()).arg(m_normal.y()).arg(m_normal.z());
    return props;
}

QVector4D Plane::surfaceNormal(const QVector4D &p, const Ray& ray)
{
    return QVector4D(m_normal, 0.0);
}

bool Plane::intersect(const Ray &ray, double &t)
{
    t = -(QVector4D::dotProduct(ray.origin(), m_n) + m_d) /
            QVector4D::dotProduct(ray.direction(), m_n);
    return t > MathUtils::dEpsilon;
}

void Plane::normalSetter(QVector3D normal)
{
    m_normal = normal;
    m_n = QVector4D(normal.normalized(), 0.0);
    calcD();
}

void Plane::pointSetter(QVector3D point)
{
    m_point = point;
    calcD();
}

void Plane::calcD()
{
    m_d = -m_normal.x() * m_point.x() - m_normal.y() * m_point.y() - m_normal.z() * m_point.z();
}
