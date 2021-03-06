/*
-- Copyright (c) 2013, Juha Turunen (turunen@iki.fi)
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
-- ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

    emittance = m_emittance;
}

void PointLight::intensitySetter(double intensity)
{
    if (intensity != m_intensity) {
        m_intensity = intensity;
        emit intensityChanged();
        m_emittance = QVector3D(m_color.redF() * m_intensity,
                                m_color.greenF() * m_intensity,
                                m_color.blueF() * m_intensity);
    }
}

void PointLight::colorSetter(QColor color)
{
    if (color != m_color) {
        m_color = color;
        emit colorChanged();
        m_emittance = QVector3D(m_color.redF() * m_intensity,
                                m_color.greenF() * m_intensity,
                                m_color.blueF() * m_intensity);
    }
}
