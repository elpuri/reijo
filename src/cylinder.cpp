#include "cylinder.h"
#include "mathutils.h"

Cylinder::Cylinder() :
    m_length(1.0),
    m_hasCaps(true)
{
}

void Cylinder::radiusSetter(qreal radius)
{
    if (radius != m_radius) {
        m_radius = radius;
        emit radiusChanged();
    }

    m_rSq = radius * radius;
}

bool Cylinder::intersect(const Ray &ray, double &t)
{
    // TODO: store m_length * 0.5
    Ray lr = ray * m_worldToObject;
    double dx = lr.direction().x();
    double dz = lr.direction().z();
    double ox = lr.origin().x();
    double oz = lr.origin().z();


    double a = dx * dx + dz * dz;
    double b = 2.0 * (dz * oz + dx * ox);
    double c = ox * ox + oz * oz - m_rSq;

    double t1, t2;
    bool cylinderHit(false);

    if (MathUtils::solveQuadratic(a, b, c, t1, t2)) {
        if (t1 > t2)
            qSwap<double>(t1, t2);

        if (t1 > MathUtils::dEpsilon) {
            double hitY = lr.origin().y() + t1 * lr.direction().y();
            if ((hitY > (-m_length * 0.5)) && (hitY < (m_length * 0.5))) {
                t = t1;
                cylinderHit = true;
            }
        }
        // Don't check the further hit if we have caps because we can't see inside anyway
        if (!cylinderHit && !m_hasCaps && t2 > MathUtils::dEpsilon){
            double hitY = lr.origin().y() + t2 * lr.direction().y();
            if ((hitY > (-m_length * 0.5)) && (hitY < (m_length * 0.5))) {
                t = t2;
                cylinderHit = true;
            }
        }
    }

    if (cylinderHit)
        return true;

    if (!m_hasCaps)
        return false;

    return intersectCap(lr, t);
}

bool Cylinder::intersectCap(const Ray &localRay, double &t)
{
    if (localRay.direction().y() < 0.0)
        t = (0.5 * m_length - localRay.origin().y()) / localRay.direction().y();
    else if (localRay.direction().y() > 0.0)
        t = (-0.5 * m_length - localRay.origin().y()) / localRay.direction().y();
    else
        return false;

    QVector4D p = localRay.along(t);
    if (p.x() * p.x() + p.z() * p.z() < m_rSq)
        return true;

    return false;
}

QVector4D Cylinder::surfaceNormal(const QVector4D &p, const Ray &ray)
{
    QVector4D lp = m_worldToObject * p;
    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));


    if (m_hasCaps && fabs(fabs(lp.y()) - m_length * 0.5) < MathUtils::dEpsilon) {
        // Return cap normal
        QVector4D n;
        if (lp.y() < 0.0)
            n = QVector4D(0.0, -1.0, 0.0, 0.0);
        else
            n = QVector4D(0.0, 1.0, 0.0, 0.0);
        return (a * n).normalized();
    } else {
        // calculate cylinder normal
        QVector4D o = QVector4D(0.0, lp.y(), 0.0, 1.0);
        QVector4D n = lp - o;
        n = a * n;

        QVector4D rd = -ray.direction();
        if (QVector4D::dotProduct(n, rd) < 0)
            return -n.normalized();
        else
            return n.normalized();
    }
}
