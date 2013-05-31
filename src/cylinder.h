#ifndef CYLINDER_H
#define CYLINDER_H

#include "proputils.h"
#include "shape.h"
#include "ray.h"

class Cylinder : public Shape
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY_WDECL(qreal, radius)
    SIMPLE_NOTIFYING_PROPERTY(qreal, length)
    SIMPLE_NOTIFYING_PROPERTY(bool, hasCaps)

public:
    Cylinder();
    bool intersect(const Ray &ray, double &t);
    QVector4D surfaceNormal(const QVector4D &p, const Ray &ray);
    bool hasBoundingBox() const { return true; }

private:
    bool intersectCap(const Ray& localRay, double &t);

private:
    qreal m_rSq;
};

#endif // CYLINDER_H
