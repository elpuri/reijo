#include "cone.h"

Cone::Cone(QObject *parent) :
    Shape(parent)
{
}

bool Cone::intersect(const Ray &ray, double &t)
{
    Ray localRay = ray * m_worldToObject;

    double dx = localRay.direction().x();
    double dy = localRay.direction().y();
    double dz = localRay.direction().z();
    double ox = localRay.origin().x();
    double oy = localRay.origin().y();
    double oz = localRay.origin().z();

    double a = dx * dx - dy * dy + dz * dz;
    double b = 2.0 * (dx * ox - dy * oy + dy + dz * oz);
    double c = ox * ox - oy * oy + 2 * oy + oz * oz - 1.0;

    double t1, t2, t3;

    // Calculate bottom disc t
    t3 = -oy / dy;
    double discX, discZ;
    discX = ox + dx * t3;
    discZ = oz + dz * t3;
    if (discX * discX + discZ * discZ > 1.0)
        t3 = -1.0;  // Didn't hit the disc

    if (MathUtils::solveQuadratic(a, b, c, t1, t2)) {
        // Set cone tees to negative if the hit point y isn't between 0 and 1
        double hy = oy + dy * t1;
        if (!(hy >= 0.0 && hy <= 1.0))
            t1 = -1.0;

        hy = oy + dy * t2;
        if (!(hy >= 0.0 && hy <= 1.0))
            t2 = -1.0;

        bool hit = false; t = std::numeric_limits<double>::max();
        if (t1 > MathUtils::dEpsilon) {
            hit = true;
            t = t1;
        }
        if (t2 < t && t2 > MathUtils::dEpsilon) {
            hit = true;
            t = t2;
        }
        if (t3 < t && t3 > MathUtils::dEpsilon) {
            hit = true;
            t = t3;
        }

        return hit;
    }

    // Can't hit bottom disc without hitting the infinite cone too
    return false;
}

QVector4D Cone::surfaceNormal(const QVector4D &p, const Ray &ray)
{
    QVector4D objectP = m_worldToObject * p;

    QVector4D n = fabs(objectP.y()) < MathUtils::dEpsilon ? QVector4D(0.0, -1.0, 0.0, 0.0) :  // We're on the base disc
                                                            QVector4D(objectP.x(), 1.0 - objectP.y(), objectP.z(), 0.0);
    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    n = a * n;
    return n.normalized();
}
