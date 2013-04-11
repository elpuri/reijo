#include "plane.h"
#include "mathutils.h"

#include <QDebug>

Plane::Plane(QObject *parent) :
    Shape(parent)
{
}


QVector4D Plane::surfaceNormal(const QVector4D &p)
{
    return QVector4D(m_normal, 0.0);
}

bool Plane::intersect(const Ray &ray, double &t)
{
    t = -(QVector4D::dotProduct(ray.origin().toVector3D(), m_normal) + m_d) /
            QVector4D::dotProduct(ray.direction().toVector3D(), m_normal);
    return t > MathUtils::dEpsilon;
}

void Plane::normalSetter(QVector3D normal)
{
    m_normal = normal.normalized();
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
