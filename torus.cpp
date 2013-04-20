#include "torus.h"
#include "mathutils.h"
#include <QDebug>

Torus::Torus(QObject *parent) :
    Shape(parent)
{
}

void Torus::classBegin()
{

}

void Torus::componentComplete()
{

}

void Torus::innerRadiusSetter(qreal innerRadius)
{
    if (m_innerRadius != innerRadius) {
        m_innerRadius = innerRadius;
        m_bSq = innerRadius * innerRadius;
        m_aSqPlusBSq = m_aSq + m_bSq;
        emit innerRadiusChanged();
    }
}

void Torus::outerRadiusSetter(qreal outerRadius)
{
    if (m_outerRadius != outerRadius) {
        m_outerRadius = outerRadius;
        m_aSq = outerRadius * outerRadius;
        m_aSqPlusBSq = m_aSq + m_bSq;
        emit outerRadiusChanged();
    }
}

qreal Torus::outerRadius() {
    return m_outerRadius;
}

qreal Torus::innerRadius() {
    return m_innerRadius;
}

bool Torus::intersect(const Ray &ray, double &t)
{
    Ray localRay = ray * m_worldToObject;


    double coeffs[5];
    double dx = localRay.direction().x();
    double dy = localRay.direction().y();
    double dz = localRay.direction().z();
    double ox = localRay.origin().x();
    double oy = localRay.origin().y();
    double oz = localRay.origin().z();

    double dDotD = dx * dx + dy * dy + dz * dz;
    double oDotO = ox * ox + oy * oy + oz * oz;
    double oDotD = ox * dx + oy * dy + oz * dz;
    double oDotOASqBSq = oDotO - m_aSq - m_bSq;
    coeffs[4] = dDotD * dDotD;
    coeffs[3] = 4.0 * dDotD * oDotD;
    coeffs[2] = 2.0 * dDotD * oDotOASqBSq + 4.0 * oDotD * oDotD + 4.0 * m_aSq * dy * dy;
    coeffs[1] = 4.0 * oDotOASqBSq * oDotD + 8.0 * m_aSq * oy * dy;
    coeffs[0] = oDotOASqBSq * oDotOASqBSq - 4.0 * m_aSq * (m_bSq - (oy * oy));

    double solutions[4];
    int solutionCount = MathUtils::solveQuartic(coeffs, solutions);
    if (solutionCount < 1)
        return false;

    double minT = 999999999999.0;
    for (int i=0; i < solutionCount; i++) {
        if (solutions[i] < minT)
            minT = solutions[i];
    }

    if (minT > MathUtils::dEpsilon) {
        t = minT;
        return true;
    }

    return false;

}

QVector4D Torus::surfaceNormal(const QVector4D &p)
{
    // Transform hit point to object space
    QVector4D objectP = m_worldToObject * p;

    // Calculate normal in object space
    double x = objectP.x(), y = objectP.y(), z = objectP.z();
    double nx = 4.0 * x * (x * x + y * y + z * z - m_aSq - m_bSq);
    double ny = 4.0 * y * (x * x + y * y + z * z - m_aSq - m_bSq + 2.0 * m_aSq);
    double nz = 4.0 * z * (x * x + y * y + z * z - m_aSq - m_bSq);
    QVector4D n(nx, ny, nz, 0.0);

    // Transform normal to world space
    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    n = a * n;
    return n.normalized();
}
