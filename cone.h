#ifndef CONE_H
#define CONE_H

#include "shape.h"
#include "ray.h"

class Cone : public Shape
{
    Q_OBJECT

public:
    explicit Cone(QObject *parent = 0);

    bool intersect(const Ray &ray, double &t);
    QVector4D surfaceNormal(const QVector4D &p, const Ray &ray);

    bool hasBoundingBox() const { return true; }
    BoundingBox objectSpaceBoundingBox() const {
        return BoundingBox(QVector4D(-1.0, 0.0, -1.0, 1.0),
                           QVector4D(1.0, 1.0, 1.0, 1.0));
    }
};

#endif // CONE_H
