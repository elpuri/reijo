#include "mathutils.h"
#include <math.h>

MathUtils::MathUtils()
{
}

const double MathUtils::dEpsilon = 1e-5d;
const float MathUtils::fEpsilon = 1e-5f;

bool MathUtils::solveQuadratic(const double a, const double b, const double c, double &t1, double &t2)
{
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    double root = sqrt(discriminant);
    t1 = (-b + root) / (2 * a);
    t2 = (-b - root) / (2 * a);
    return true;
}

QVector4D MathUtils::reflect(const QVector4D &incident, const QVector4D normal)
{
    return incident - 2 * QVector4D::dotProduct(incident, normal) * normal;
}

QVector4D MathUtils::refract(const QVector4D &incident, const float n1, const float n2)
{

}
