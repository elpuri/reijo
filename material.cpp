#include "material.h"

Material::Material(QObject *parent) :
    QObject(parent) ,
    m_luminance(0.0),
    m_ambientReflectivity(0.0),
    m_diffuseReflectivity(0.0),
    m_specularReflectivity(0.0),
    m_shininess(3.0),
    m_reflectivity(0.0)
{
}
