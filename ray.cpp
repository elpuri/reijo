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
