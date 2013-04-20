#ifndef DISC_H
#define DISC_H

#include "shape.h"
#include "proputils.h"

class Disc : public Shape
{
    Q_OBJECT

    SIMPLE_PROPERTY_WDECL(QVector3D, normal)
    SIMPLE_PROPERTY_WDECL(QVector3D, origin)
    SIMPLE_NOTIFYING_PROPERTY(qreal, radius)

public:
    explicit Disc(QObject *parent = 0);
    
    bool intersect(const Ray &ray, double &t);
    QVector4D surfaceNormal(const QVector4D &p, const Ray& ray);

private:
    void calcD();

private:
    qreal m_d;
};

#endif // DISC_H
