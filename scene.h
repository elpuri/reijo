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
