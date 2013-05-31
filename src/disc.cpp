#include "disc.h"
#include "mathutils.h"

Disc::Disc(QObject *parent) :
    Shape(parent)
{
}


QVector4D Disc::surfaceNormal(const QVector4D &p, const Ray& ray)
{
    QVector4D v = ray.origin() - p;
    if (QVector4D::dotProduct(v, m_normal) < 0)
        return QVector4D(-m_normal, 0.0);
    else
        return QVector4D(m_normal, 0.0);
}

bool Disc::intersect(const Ray &ray, double &t)
{
    Ray localRay = ray * m_worldToObject;
    t = -(QVector4D::dotProduct(localRay.origin().toVector3D(), m_normal) + m_d) /
            QVector4D::dotProduct(localRay.direction().toVector3D(), m_normal);
    if (t > MathUtils::dEpsilon) {
        // We're on the plane
        QVector4D p = localRay.along(t);
        if ((p - m_origin).lengthSquared() < m_radius * m_radius)
            return true;
    }

    return false;
}

void Disc::normalSetter(QVector3D normal)
{
    m_normal = normal.normalized();
    calcD();
}

void Disc::originSetter(QVector3D point)
{
    m_origin = point;
    calcD();
}

void Disc::calcD()
{
    m_d = -m_normal.x() * m_origin.x() - m_normal.y() * m_origin.y() - m_normal.z() * m_origin.z();
}
