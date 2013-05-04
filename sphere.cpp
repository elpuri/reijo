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

#include "sphere.h"
#include "mathutils.h"


Sphere::Sphere(QObject* parent) :
    Shape(parent),
    m_radius(0.0)
{
}

QString Sphere::dumpProperties()
{
    QString shapeProps = Shape::dumpProperties();

    QString props("[radius: ");
    props += QString::number(m_radius) + "]";
    return shapeProps +  " " + props;
}

bool Sphere::intersect(const Ray &ray, double &t)
{
    Ray localRay = ray * m_worldToObject;

    double dx = localRay.direction().x();
    double dy = localRay.direction().y();
    double dz = localRay.direction().z();
    double ox = localRay.origin().x();
    double oy = localRay.origin().y();
    double oz = localRay.origin().z();

    double a = dx * dx + dy * dy + dz * dz;
    double b = 2.0 * (dx * ox + dy * oy + dz * oz);
    double c = ox * ox + oy * oy + oz * oz - m_radius * m_radius;

    double t1, t2;
    if (MathUtils::solveQuadratic(a, b, c, t1, t2)) {
        if (t1 > t2)
            qSwap<double>(t1, t2);
        if (t1 > MathUtils::dEpsilon) {
            t = t1;
            return true;
        }

        if (t2 > MathUtils::dEpsilon) {
            t = t2;
            return true;
        }
    }
    return false;
}

QVector4D Sphere::surfaceNormal(const QVector4D &p, const Ray& ray)
{
    QVector4D origin(0.0, 0.0, 0.0, 1.0);
    QVector4D objectP = m_worldToObject * p;
    QVector4D n = objectP - origin;
    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    n = a * n;

    return n.normalized();
}

BoundingBox Sphere::worldSpaceBoundingBox() const
{
    if (m_scale != QVector3D(1.0, 1.0, 1.0))
        return Shape::worldSpaceBoundingBox();

    QVector4D o(0.0, 0.0, 0.0, 1.0);
    o = m_objectToWorld * o;
    BoundingBox bb(o - QVector4D(m_radius, m_radius, m_radius, 0.0),
                       o + QVector4D(m_radius, m_radius, m_radius, 0.0));
    return bb;
}
