#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include "scenenode.h"

class Light : public SceneNode
{
    Q_OBJECT

public:
    Light(QObject* parent = 0);

    virtual void sample(QVector4D& position, QVector3D& emittance) = 0;
};

#endif // LIGHT_H
