#ifndef REGULARGRID_H
#define REGULARGRID_H

#include <QList>
#include "shape.h"
#include "boundingbox.h"

class RegularGrid
{
    struct ShapeBoxPair { Shape* s; BoundingBox wsbb; };

public:

    RegularGrid();
    void initialize(QList<Shape*>& shapes);
    QList<Shape*> alwaysIntersect();
    void worldCoordinatesToGridIndices(const QVector4D c, int& x, int& y, int& z) const;
    bool intersect(const Ray& r, Shape*& s, double& t) const;

private:
    bool intersectBoundless(const Ray& r, Shape*& s, double& t) const;
    void addToGrid(const RegularGrid::ShapeBoxPair s);
    int gridIndex(const int x, const int y, const int z) const {
        return z * m_dimX * m_dimY + y * m_dimX + x;
    }

private:
    QList<Shape*> m_alwaysIntersect;
    BoundingBox m_gridBB;
    QVector3D m_gridBBSize;
    QVector3D m_cellSize;
    QVector<Shape*>* m_cells;
    int m_dimX, m_dimY, m_dimZ;
};

#endif // REGULARGRID_H
