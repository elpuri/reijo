#include "cylinder.h"
#include "mathutils.h"

Cylinder::Cylinder() :
    m_length(1.0)
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
    Ray lr = ray * m_worldToObject;
    double dx = lr.direction().x();
    double dz = lr.direction().z();
    double ox = lr.origin().x();
    double oz = lr.origin().z();


    double a = dx * dx + dz * dz;
    double b = 2.0 * (dz * oz + dx * ox);
    double c = ox * ox + oz * oz - m_rSq;

    double t1, t2;
    if (MathUtils::solveQuadratic(a, b, c, t1, t2)) {
        if (t1 > t2)
            qSwap<double>(t1, t2);

        if (t1 > MathUtils::dEpsilon) {
            double hitY = lr.origin().y() + t1 * lr.direction().y();
            if ((hitY > (-m_length * 0.5)) && (hitY < (m_length * 0.5))) {
                t = t1;
                return true;
            }
        }
        if (t2 > MathUtils::dEpsilon){
            double hitY = lr.origin().y() + t2 * lr.direction().y();
            if ((hitY > (-m_length * 0.5)) && (hitY < (m_length * 0.5))) {
                t = t2;
                return true;
            }
        }
    }
    return false;
}

QVector4D Cylinder::surfaceNormal(const QVector4D &p, const Ray &ray)
{
    QVector4D lp = m_worldToObject * p;
    QVector4D o = QVector4D(0.0, lp.y(), 0.0, 1.0);
    QVector4D n = lp - o;
    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    n = a * n;

    QVector4D rd = -ray.direction();
    if (QVector4D::dotProduct(n, rd) < 0)
        return -n.normalized();
    else
        return n.normalized();
}
