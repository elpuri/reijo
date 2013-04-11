#ifndef SPHERE_H
#define SPHERE_H

#include "proputils.h"
#include "shape.h"

class Sphere : public Shape
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY(qreal, radius)

public:
    explicit Sphere(QObject* parent = 0);

    bool intersect(const Ray &ray, double &t);
    QVector4D surfaceNormal(const QVector4D &p);

protected:
    QString dumpProperties();
};

#endif // SPHERE_H
