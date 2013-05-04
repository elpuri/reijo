#include "boundingbox.h"
#include "mathutils.h"

bool BoundingBox::intersect(const Ray r, double& t) const
{
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    double ox = r.origin().x(), oy = r.origin().y(), oz = r.origin().z();
    double dx = r.direction().x(), dy = r.direction().y(), dz = r.direction().z();
    double minx = m_blf.x(), maxx = m_trb.x();
    double miny = m_blf.y(), maxy = m_trb.y();
    double minz = m_blf.z(), maxz = m_trb.z();

    if (dx >= 0) {
       tmin = (minx - ox) / dx;
       tmax = (maxx - ox) / dx;
    } else {
        tmin = (maxx - ox) / dx;
        tmax = (minx - ox) / dx;
    }

    if (r.direction().y() >= 0) {
        tymin = (miny - oy) / dy;
        tymax = (maxy - oy) / dy;
    } else {
        tymin = (maxy - oy) / dy;
        tymax = (miny - oy) / dy;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    if (dz >= 0) {
        tzmin = (minz - oz) / dz;
        tzmax = (maxz - oz) / dz;
    } else {
        tzmin = (maxz - oz) / dz;
        tzmax = (minz - oz) / dz;
    }
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tmin > MathUtils::dEpsilon) {
        t = tmin;
        return true;
    }

    return false;
}

QDebug operator<<(QDebug dbg, const BoundingBox& b) {
    return dbg << "Bounding box:" << b.m_blf << "," << b.m_trb;
}

BoundingBox BoundingBox::transform(const QMatrix4x4 transform) const
{
    QVector4D p[8];
    // Front first clockwise starting from bottom left
    p[0] = m_blf;
    p[1] = QVector4D(m_blf.x(), m_trb.y(), m_blf.z(), 1.0);
    p[2] = QVector4D(m_trb.x(), m_trb.y(), m_blf.z(), 1.0);
    p[3] = QVector4D(m_trb.x(), m_blf.y(), m_blf.z(), 1.0);
    // Then back clockwise starting from top right
    p[4] = m_trb;
    p[5] = QVector4D(m_trb.x(), m_blf.y(), m_trb.z(), 1.0);
    p[6] = QVector4D(m_blf.x(), m_blf.y(), m_trb.z(), 1.0);
    p[7] = QVector4D(m_blf.x(), m_trb.y(), m_trb.z(), 1.0);

    double minX, minY, minZ, maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<double>::max();
    maxX = maxY = maxZ = -std::numeric_limits<double>::max();

    for (int i=0; i < 8; i++) {
       QVector4D t = transform * p[i];
       if (t.x() >= maxX) maxX = t.x();
       if (t.x() < minX) minX = t.x();
       if (t.y() >= maxY) maxY = t.y();
       if (t.y() < minY) minY = t.y();
       if (t.z() >= maxZ) maxZ = t.z();
       if (t.z() < minZ) minZ = t.z();
    }

    return BoundingBox(QVector4D(minX, minY, minZ, 1.0),
                       QVector4D(maxX, maxY, maxZ, 1.0));
   }
