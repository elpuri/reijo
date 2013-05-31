#include "regulargrid.h"
#include <QVector>
#include <assert.h>
#include <limits>
#include <math.h>

#include "mathutils.h"

RegularGrid::RegularGrid()
{
}

void RegularGrid::initialize(QList<Shape *> &shapes)
{
    // The number of unbounded objects is usually small or none so we're not allocating too many extra elements
    QVector<ShapeBoxPair> boundableShapes;
    boundableShapes.reserve(shapes.count());
    bool first = true;
    foreach(Shape* s, shapes) {
        if (s->hasBoundingBox()) {
            ShapeBoxPair sbp;
            sbp.s = s;
            sbp.wsbb = s->worldSpaceBoundingBox();
            boundableShapes.append(sbp);
            if (first) {
                m_gridBB = sbp.wsbb;
                first = false;
            } else
                m_gridBB.merge(sbp.wsbb);
        } else
            m_alwaysIntersect.append(s);
    }
    m_gridBB.m_blf -= MathUtils::v4Epsilon;
    m_gridBB.m_trb += MathUtils::v4Epsilon;

    m_gridBBSize = (m_gridBB.m_trb - m_gridBB.m_blf).toVector3D();

    qDebug() << "World bounding box" << m_gridBB.m_blf.toVector3D() << m_gridBB.m_trb.toVector3D() << ", size:" << m_gridBBSize;

    int m = 3;
    double s = pow(m_gridBBSize.x() * m_gridBBSize.y() * m_gridBBSize.z() / boundableShapes.count(), 1.0 / 3.0);
    m_dimX = ((int) (m * m_gridBBSize.x() / s)) + 1;
    m_dimY = ((int) (m * m_gridBBSize.y() / s)) + 1;
    m_dimZ = ((int) (m * m_gridBBSize.z() / s)) + 1;


    m_cellSize.setX(m_gridBBSize.x() / m_dimX);
    m_cellSize.setY(m_gridBBSize.y() / m_dimY);
    m_cellSize.setZ(m_gridBBSize.z() / m_dimZ);
    qDebug() << "Creating grid with dimensions:" << m_dimX << "x" << m_dimY << "x" << m_dimZ << "shape count" << boundableShapes.count();

    m_cells = new QVector<Shape*>[m_dimX * m_dimY * m_dimZ];

    // Add shapes to cells
    foreach(ShapeBoxPair s, boundableShapes) {

        int blfx, blfy, blfz, trbx, trby, trbz;
        worldCoordinatesToGridIndices(s.wsbb.m_blf, blfx, blfy, blfz);
        worldCoordinatesToGridIndices(s.wsbb.m_trb, trbx, trby, trbz);
        for (int i = blfx; i <= trbx; i++) {
            for (int j = blfy; j <= trby; j++) {
                for (int k = blfz; k <= trbz; k++) {
                    m_cells[gridIndex(i, j, k)].append(s.s);
                }
            }
        }
    }

    // Calc stats
    int minShapeCount = 99999999999, maxShapeCount = 0, total = 0;
    for (int i = 0; i < m_dimX; i++) {
        for (int j = 0; j < m_dimY; j++) {
            for (int k = 0; k < m_dimZ; k++) {
                int c = m_cells[gridIndex(i, j, k)].count();
                maxShapeCount = maxShapeCount < c ? c : maxShapeCount;
                minShapeCount = minShapeCount > c ? c : minShapeCount;
                total += c;
            }
        }
    }

    qDebug() << "Most populated cell has:" << maxShapeCount << ", Least populated cell has" << minShapeCount <<
                "Average population:" << (float) total / (m_dimX * m_dimY * m_dimZ);
}

bool RegularGrid::intersectBoundless(const Ray &r, Shape *&s, double &t) const
{
    double minT = std::numeric_limits<double>::max();
    bool hit = false;
    foreach(Shape* bs, m_alwaysIntersect) {
        if (bs->intersect(r, t)) {
            if (t > MathUtils::dEpsilon && t < minT) {
                s = bs;
                minT = t;
                hit = true;
            }
        }
    }
    t = minT;
    return hit;
}


bool RegularGrid::intersect(const Ray &r, Shape *&hitShape, double &minT) const
{
    // http://www.cse.chalmers.se/edu/course/_MY_MISSING_COURSE_2012/_courses_2011/TDA361_Computer_Graphics/grid.pdf
    double rdx = r.direction().x(), rdy = r.direction().y(), rdz = r.direction().z();
    QVector4D cellEnter;
    double tt = 0;
    if (m_gridBB.isInside(r.origin())) {
        cellEnter = r.origin();
    } else {
        // Check if the ray even hits the grid bounding box
        if (m_gridBB.intersect(r, tt)) {
            // Figure out the first cell hit
            cellEnter = r.along(tt);
        } else
            return intersectBoundless(r, hitShape, minT);
    }

    bool boundlessHit = intersectBoundless(r, hitShape, minT);

    double tMaxX, tMaxY, tMaxZ;  // maximum t value before hitting a wall of a cell
    int cellX, cellY, cellZ;    // currently traversed grid cell coordinates
    int stepX, stepY, stepZ;    // sign of the ray direction
    // How much t is needed to skip once cell in x/y/z direction. Ray direction is unit vector, so units of t are the same as grid coordinates
    double tDeltaX, tDeltaY, tDeltaZ;

    worldCoordinatesToGridIndices(cellEnter, cellX, cellY, cellZ);
    auto rayOcellO = r.origin() - m_gridBB.m_blf;

    if (rdx > 0.0) {
        stepX = 1;
        tDeltaX = m_cellSize.x() / rdx;
        tMaxX = ((cellX + 1) * m_cellSize.x() - rayOcellO.x()) / rdx;
    } else {
        stepX = -1;
        tDeltaX = m_cellSize.x() / -rdx;
        tMaxX = (cellX * m_cellSize.x() - rayOcellO.x()) / rdx;    // the nominator and denominator are both negative so they cancel out
    }

    if (rdy > 0.0) {
        stepY = 1;
        tDeltaY = m_cellSize.y() / rdy;
        tMaxY = ((cellY + 1) * m_cellSize.y() - rayOcellO.y()) / rdy;
    } else {
        stepY = -1;
        tDeltaY = m_cellSize.y() / -rdy;
        tMaxY = (cellY * m_cellSize.y() - rayOcellO.y()) / rdy;    // the nominator and denominator are both negative so they cancel out
    }

    if (rdz > 0.0) {
        stepZ = 1;
        tDeltaZ = m_cellSize.z() / rdz;
        tMaxZ = ((cellZ + 1) * m_cellSize.z() - rayOcellO.z()) / rdz;
    } else {
        stepZ = -1;
        tDeltaZ = m_cellSize.z() / -rdz;
        tMaxZ = (cellZ * m_cellSize.z() - rayOcellO.z()) / rdz;    // the nominator and denominator are both negative so they cancel out
    }

    bool bailOut = false;
    while(!bailOut) {
        double t;
        double cellBorderT = MIN3(tMaxX, tMaxY, tMaxZ);
        bool hitFound = false;
        QVector<Shape*> cellContent = m_cells[gridIndex(cellX, cellY, cellZ)];
        foreach(Shape* s, cellContent) {
            if (s->intersect(r, t)) {
                if (t > MathUtils::dEpsilon && t < minT && t < cellBorderT) {
                    minT = t;
                    hitShape = s;
                    hitFound = true;
                }
            }
        }

        if (hitFound)
            return true;

        if (cellBorderT == tMaxX) {
            cellX += stepX;
            if (cellX < 0 || cellX >= m_dimX)
                bailOut = true;
            tMaxX += tDeltaX;
        } else if (cellBorderT == tMaxY) {
            cellY += stepY;
            if (cellY < 0 || cellY >= m_dimY)
                bailOut = true;
            tMaxY += tDeltaY;
        } else {
            cellZ += stepZ;
            if (cellZ < 0 || cellZ >= m_dimZ)
                bailOut = true;
            tMaxZ += tDeltaZ;
        }
    }
    return boundlessHit;
}


void RegularGrid::worldCoordinatesToGridIndices(const QVector4D c, int &x, int &y, int &z) const
{
    auto v = c - m_gridBB.m_blf;
    x = floor(v.x() / m_cellSize.x());
    y = floor(v.y() / m_cellSize.y());
    z = floor(v.z() / m_cellSize.z());
    x = qBound<int>(0, x, m_dimX - 1);
    y = qBound<int>(0, y, m_dimY - 1);
    z = qBound<int>(0, z, m_dimZ - 1);
}
