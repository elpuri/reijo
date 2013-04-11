/*
-- Copyright (c) 2013, Juha Turunen (turunen@iki.fi)
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
-- ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
