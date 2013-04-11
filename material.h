#ifndef MATERIAL_H
#define MATERIAL_H

#include <QObject>
#include <QColor>
#include <QVector3D>

#include "proputils.h"

class Material : public QObject
{
    Q_OBJECT

    SIMPLE_NOTIFYING_PROPERTY(float, luminance)
    SIMPLE_NOTIFYING_PROPERTY(float, ambientReflectivity)
    SIMPLE_NOTIFYING_PROPERTY(float, diffuseReflectivity)
    SIMPLE_NOTIFYING_PROPERTY(float, specularReflectivity)
    SIMPLE_NOTIFYING_PROPERTY(float, reflectivity)
    SIMPLE_NOTIFYING_PROPERTY(float, transmittance)
    SIMPLE_NOTIFYING_PROPERTY(float, indexOfRefraction)
    SIMPLE_NOTIFYING_PROPERTY(QColor, color)
    SIMPLE_NOTIFYING_PROPERTY(float, shininess)

public:
    explicit Material(QObject *parent = 0);
    QVector3D colorVector() { return QVector3D(m_color.redF(), m_color.greenF(), m_color.blueF()); }
    
signals:
    
public slots:
    
};

#endif // MATERIAL_H
