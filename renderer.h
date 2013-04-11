#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QString>
#include "proputils.h"

class Scene;

class Renderer : public QObject
{
    Q_OBJECT

    SIMPLE_PROPERTY(int, renderedWidth)
    SIMPLE_PROPERTY(int, renderedHeight)

public:

    Renderer(QObject* parent = 0);

    virtual void render(Scene* s) = 0;

    virtual void copyRenderBuffer(float*& buffer) = 0;

signals:
    void progressiveUpdate();
    void frameComplete();



};

#endif // RENDERER_H
