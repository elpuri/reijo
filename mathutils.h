#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QVector4D>

class MathUtils
{
public:
    static bool solveQuadratic(const double a, const double b, const double c,
                               double &t1, double &t2);

    static QVector4D reflect(const QVector4D& incident, const QVector4D normal);
    static QVector4D refract(const QVector4D& incident, const float n1, const float n2);
    static const double dEpsilon;
    static const float fEpsilon;

private:
    MathUtils();
};

#endif // MATHUTILS_H
