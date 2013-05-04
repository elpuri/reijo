#ifndef BOX_H
#define BOX_H

#include "shape.h"
#include "ray.h"

class Box : public Shape
{
    Q_OBJECT

    // By default the box's extens are (-1, -1, -1), (1, 1, 1)
    // Use Shape's scale property to define it's dimensions

public:
    explicit Box(QObject *parent = 0);

    bool intersect(const Ray &r, double &t);
    QVector4D surfaceNormal(const QVector4D &p, const Ray& ray);
    bool hasBoundingBox() const { return true; }
    BoundingBox objectSpaceBoundingBox() const {
        return BoundingBox(QVector4D(-1.0, -1.0, -1.0, 1.0), QVector4D(1.0, 1.0, 1.0, 1.0));
    }
};

#endif // BOX_H
