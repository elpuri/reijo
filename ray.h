#ifndef RAY_H
#define RAY_H

#include <QVector4D>

class Ray
{
public:
    Ray();
    Ray(const QVector4D& o, const QVector4D& d);
    void set(const QVector4D& o, const QVector4D& d);

    QVector4D origin() const { return m_o; }
    QVector4D direction() const { return m_d; }
    QVector4D along(double t) const;

    void dump();

    Ray operator*(const QMatrix4x4& m) const;
    friend QDebug operator<<(QDebug dbg, const Ray &r);
private:
    QVector4D m_o;
    QVector4D m_d;

};

#endif // RAY_H
