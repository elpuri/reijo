#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>


#include "scenenode.h"

class Shape;
class Camera;
class Light;
class Renderer;

class Scene : public SceneNode
{
    Q_OBJECT

    SIMPLE_PROPERTY(Camera*, camera)
    SIMPLE_PROPERTY(Renderer*, renderer)

public:
    explicit Scene(QObject *parent = 0);
    
    Q_INVOKABLE void dumpNodes();
    Q_INVOKABLE void render();


    QList<Shape*> gatherShapes();
    QList<Light*> gatherLights();
    Camera* activeCamera();

private:
    void buildTransformationMatrices();
    Renderer* activeRenderer();

    static void calculateCompositeMatrix(const QMatrix4x4 parentMatrix, Shape* shape);
    static void addChildShapes(QList<Shape*>& shapes, SceneNode* node);

private:
};

#endif // SCENE_H
