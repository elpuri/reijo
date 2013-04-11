#ifndef PLANE_H
#define PLANE_H

#include "shape.h"
#include "proputils.h"

class Plane : public Shape
{
    Q_OBJECT

    SIMPLE_PROPERTY_WDECL(QVector3D, normal)
    SIMPLE_PROPERTY_WDECL(QVector3D, point)

public:
    explicit Plane(QObject *parent = 0);

    bool intersect(const Ray &ray, double &t);
    QVector4D surfaceNormal(const QVector4D &p);

private:
    void calcD();


private:
    double m_d;

    
};

#endif // PLANE_H
