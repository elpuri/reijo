#include <assert.h>

#include "shape.h"

Shape::Shape(QObject* parent) :
    SceneNode(parent),
    m_scale(1.0, 1.0, 1.0),
    m_position(0.0, 0.0, 0.0),
    m_castsShadows(true)
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
    m_transformation.setToIdentity();
    m_transformation.translate(m_position);
    m_transformation.rotate(m_rotation.x(), 1.0, 0.0, 0.0);
    m_transformation.rotate(m_rotation.y(), 0.0, 1.0, 0.0);
    m_transformation.rotate(m_rotation.z(), 0.0, 0.0, 1.0);
    m_transformation.scale(m_scale);
}

void Shape::classBegin()
{

}

void Shape::applyParentMatrix(const QMatrix4x4 m)
{
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
