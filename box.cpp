#include "box.h"
#include "mathutils.h"
#include "math.h"

Box::Box(QObject *parent) :
    Shape(parent)
{
}

bool Box::intersect(const Ray &ray, double &t)
{
    Ray r = ray * m_worldToObject;

    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    double ox = r.origin().x(), oy = r.origin().y(), oz = r.origin().z();
    double dx = r.direction().x(), dy = r.direction().y(), dz = r.direction().z();



    if (dx >= 0) {
       tmin = (-1.0d - ox) / dx;
       tmax =  (1.0d - ox) / dx;
    } else {
        tmin = ( 1.0d - ox) / dx;
        tmax = (-1.0d - ox) / dx;
    }

    if (r.direction().y() >= 0) {
        tymin = (-1.0d - oy) / dy;
        tymax = ( 1.0d - oy) / dy;
    } else {
        tymin = ( 1.0d - oy) / dy;
        tymax = (-1.0d - oy) / dy;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    if (dz >= 0) {
        tzmin = (-1.0d - oz) / dz;
        tzmax = ( 1.0d - oz) / dz;
    } else {
        tzmin = ( 1.0d - oz) / dz;
        tzmax = (-1.0d - oz) / dz;
    }
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tmin > MathUtils::fEpsilon) {
        t = tmin;
        return true;
    }

    return false;

}

QVector4D Box::surfaceNormal(const QVector4D &p, const Ray &ray)
{
    Q_UNUSED(ray)

    // Transform hit point to object space
    QVector4D op = m_worldToObject * p;

    QVector4D n;

    if (op.x() > -1.0f - MathUtils::dEpsilon && op.x() < -1.0 + MathUtils::dEpsilon)
        n = QVector4D(-1.0, 0.0, 0.0, 0.0);
    else if (op.x() > 1.0f - MathUtils::dEpsilon && op.x() < 1.0 - MathUtils::dEpsilon)
        n = QVector4D(1.0, 0.0, 0.0, 0.0);
    else if (op.y() > -1.0f - MathUtils::dEpsilon && op.y() < -1.0 + MathUtils::dEpsilon)
        n = QVector4D(0.0, -1.0, 0.0, 0.0);
    else if (op.y() > 1.0f - MathUtils::dEpsilon && op.y() < 1.0 - MathUtils::dEpsilon)
        n = QVector4D(0.0, 1.0, 0.0, 0.0);
    else
        n = QVector4D(0.0, 0.0, op.z() < 0 ? -1.0 : 1.0, 0.0);

    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    n = a * n;
    return n.normalized();
}
