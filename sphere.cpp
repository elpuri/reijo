#include "sphere.h"
#include "mathutils.h"


Sphere::Sphere(QObject* parent) :
    Shape(parent),
    m_radius(0.0)
{
}

QString Sphere::dumpProperties()
{
    QString shapeProps = Shape::dumpProperties();

    QString props("[radius: ");
    props += QString::number(m_radius) + "]";
    return shapeProps +  " " + props;
}

bool Sphere::intersect(const Ray &ray, double &t)
{
    Ray localRay = ray * m_worldToObject;

    double dx = localRay.direction().x();
    double dy = localRay.direction().y();
    double dz = localRay.direction().z();
    double ox = localRay.origin().x();
    double oy = localRay.origin().y();
    double oz = localRay.origin().z();

    double a = dx * dx + dy * dy + dz * dz;
    double b = 2.0 * (dx * ox + dy * oy + dz * oz);
    double c = ox * ox + oy * oy + oz * oz - m_radius * m_radius;

    double t1, t2;
    if (MathUtils::solveQuadratic(a, b, c, t1, t2)) {
        t = fmin(t1, t2);
        if (t > MathUtils::dEpsilon)
        return true;
    }

    return false;
}

QVector4D Sphere::surfaceNormal(const QVector4D &p)
{
    QVector4D origin(0.0, 0.0, 0.0, 1.0);
    QVector4D objectP = m_worldToObject * p;
    QVector4D n = objectP - origin;
    QMatrix4x4 a = m_worldToObject.transposed();
    a.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    n = a * n;

    return n.normalized();
}
