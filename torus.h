#ifndef TORUS_H
#define TORUS_H

#include "shape.h"
#include "proputils.h"

class Torus : public Shape
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY_DECL(qreal, innerRadius)
    SIMPLE_NOTIFYING_PROPERTY_DECL(qreal, outerRadius)

public:
    explicit Torus(QObject *parent = 0);


    virtual bool intersect(const Ray& ray, double& t);
    virtual QVector4D surfaceNormal(const QVector4D& p);

    void classBegin();
    void componentComplete();

private:
    qreal m_innerRadius;
    qreal m_outerRadius;
    qreal m_aSq;
    qreal m_bSq;
    qreal m_aSqPlusBSq;

};

#endif // TORUS_H
