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

#include <assert.h>
#include "shape.h"

Shape::Shape(QObject* parent) :
    SceneNode(parent),
    m_scale(1.0, 1.0, 1.0),
    m_position(0.0, 0.0, 0.0),
    m_castsShadows(true),
    m_visible(true)
{
}


QString Shape::dumpProperties()
{
    QString props = "[pos: %1, %2, %3]";
    props = props.arg(m_position.x()).arg(m_position.y()).arg(m_position.z());
    return props;
}

void Shape::componentComplete()
{

    emit positionChanged();
}

void Shape::classBegin()
{

}

void Shape::applyParentMatrix(const QMatrix4x4 m)
{
    m_transformation.setToIdentity();
    m_transformation.translate(m_position);
    m_transformation.rotate(m_rotation.x(), 1.0, 0.0, 0.0);
    m_transformation.rotate(m_rotation.y(), 0.0, 1.0, 0.0);
    m_transformation.rotate(m_rotation.z(), 0.0, 0.0, 1.0);
    m_transformation.scale(m_scale);

    m_objectToWorld = m * m_transformation;
    bool invertible;
    m_worldToObject = m_objectToWorld.inverted(&invertible);
    assert(invertible);
/*
    qDebug() << "Trans:" << m_transformation;
    qDebug() << "Comp trans:" << m_compositeTransformation;
    qDebug() << "Inv comp trans:" << m_inverseCompositeTransformation;
*/
}
