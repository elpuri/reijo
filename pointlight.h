#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <QVector3D>
#include <QVector4D>
#include <QColor>


#include "proputils.h"
#include "light.h"

class PointLight : public Light
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY(QVector3D, position)
    SIMPLE_NOTIFYING_PROPERTY(QColor, color)
    SIMPLE_NOTIFYING_PROPERTY(double, intensity)

public:
    explicit PointLight(QObject *parent = 0);
    
    void sample(QVector4D& position, QVector3D& emittance);

private:
    QVector3D m_emittance;
};

#endif // POINTLIGHT_H
