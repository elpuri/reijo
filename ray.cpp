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

#include "ray.h"
#include <QMatrix4x4>
#include <QDebug>

Ray::Ray()
{
}


Ray::Ray(const QVector4D &o, const QVector4D &d)
{
    set(o, d);
}

void Ray::set(const QVector4D &o, const QVector4D &d)
{
    m_o = o;
    m_d = d.normalized();
}

void Ray::dump()
{
    qDebug() << "Ray -- pos:" << m_o << "direction:" << m_d;
}

Ray Ray::operator*(const QMatrix4x4& m) const
{
    Ray r;
    r.m_o = m * m_o;
    r.m_d = m * m_d;
    return r;
}

QVector4D Ray::along(double t) const
{
    return m_o + t * m_d;
}

QDebug operator<<(QDebug dbg, const Ray& r) {
    return dbg << "Ray( origin:" << r.m_o << ", direction:" << r.m_d << ")";

}
