#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector4D>
#include <math.h>
#include <QMatrix4x4>
#include "ray.h"
#include "mathutils.h"

class BoundingBox
{
public:

    // Bottom left front , top right back
    BoundingBox() {}
    BoundingBox(const QVector4D blf, const QVector4D trb) {
        m_blf = blf; m_trb = trb;
    }

    void merge(const BoundingBox bb) {
        m_blf.setX(fmin(m_blf.x(), bb.m_blf.x()));
        m_blf.setY(fmin(m_blf.y(), bb.m_blf.y()));
        m_blf.setZ(fmin(m_blf.z(), bb.m_blf.z()));
        m_trb.setX(fmax(m_trb.x(), bb.m_trb.x()));
        m_trb.setY(fmax(m_trb.y(), bb.m_trb.y()));
        m_trb.setZ(fmax(m_trb.z(), bb.m_trb.z()));
    }

    BoundingBox transform(const QMatrix4x4 transform) const;

    bool isInside(const QVector4D point) const {
        return point.x() >= m_blf.x() - 0.0 && point.x() <= m_trb.x() + 0.0 &&
               point.y() >= m_blf.y() - 0.0 && point.y() <= m_trb.y() + 0.0 &&
               point.z() >= m_blf.z() - 0.0 && point.z() <= m_trb.z() + 0.0;
    }

    bool intersect(const Ray r, double& t) const;
    friend QDebug operator<<(QDebug dbg, const BoundingBox &b);

    QVector4D m_blf;
    QVector4D m_trb;
};

#endif // BOUNDINGBOX_H
