#include "pointlight.h"

PointLight::PointLight(QObject *parent) :
    Light(parent),
    m_color(Qt::white),
    m_intensity(1.0f)
{
}

void PointLight::sample(QVector4D &position, QVector3D &emittance)
{
    position = m_position.toVector4D();
    position.setW(1.0f);

    emittance = QVector3D(m_color.redF() * m_intensity,
                          m_color.greenF() * m_intensity,
                          m_color.blueF() * m_intensity);
}
