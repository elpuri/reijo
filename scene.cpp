#include "scene.h"
#include "shape.h"
#include "scenenode.h"
#include "camera.h"
#include "renderer.h"
#include "light.h"
#include <QDebug>

Scene::Scene(QObject *parent) :
    SceneNode(parent),
    m_renderer(nullptr),
    m_camera(nullptr)
{
}

void Scene::dumpNodes()
{
    dump(0);
}

void Scene::render()
{
    buildTransformationMatrices();

    Renderer* r = activeRenderer();
    if (r) {
        Camera* c = activeCamera();
        if (!c) {
            qCritical() << "Scene: No camera found";
            return;
        }
        r->render(this);
    } else
        qCritical() << "Scene: No renderer found";
}

Camera* Scene::activeCamera()
{
    Camera* c = camera();
    if (c)
        return c;

    c = findFirstNodeOfType<Camera>(this);
    if (c)
        qDebug() << "Scene: No camera explicitly defined for the scene, using the first found Camera";
    return c;
}

Renderer* Scene::activeRenderer()
{
    Renderer* r = renderer();
    if (r)
        return r;
    r = findChild<Renderer*>();
    if (r)
        qDebug() << "Scene: No renderer explicitly defined for the scene, using the first found Renderer";
    return r;
}

QList<Shape*> Scene::gatherShapes()
{
    QList<Shape*> shapes;
    gatherNodeOfType<Shape>(shapes, this);
    return shapes;
}


QList<Light*> Scene::gatherLights()
{
    QList<Light*> lights;
    gatherNodeOfType<Light>(lights, this);
    return lights;
}

void Scene::buildTransformationMatrices()
{
    QMatrix4x4 t;

    foreach(SceneNode* node, childNodes()) {
        Shape* s = qobject_cast<Shape*>(node);
        if (s)
            calculateCompositeMatrix(t, s);
    }
}

void Scene::calculateCompositeMatrix(const QMatrix4x4 parentMatrix, Shape *shape)
{
    shape->applyParentMatrix(parentMatrix);

    foreach(SceneNode* node, shape->childNodes()) {
        Shape* s = qobject_cast<Shape*>(node);
        if (s)
            calculateCompositeMatrix(shape->compositeTransformation(), s);
    }
}
