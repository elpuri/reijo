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

#include <QApplication>
#include <QDebug>
#include <QtQml>

#include "scene.h"
#include "camera.h"
#include "shapefactory.h"
#include "shape.h"
#include "sphere.h"
#include "whittedrenderer.h"
#include "hdrviewer.h"
#include "light.h"
#include "pointlight.h"
#include "material.h"
#include "plane.h"
#include "mathutils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<Scene>("Reijo", 1, 0, "Scene");
    qmlRegisterType<Camera>("Reijo", 1, 0, "Camera");
    qmlRegisterType<ShapeFactory>("Reijo", 1, 0, "ShapeFactory");
    qmlRegisterType<Shape>("Reijo", 1, 0, "NullShape");
    qmlRegisterType<Sphere>("Reijo", 1, 0, "Sphere");
    qmlRegisterType<WhittedRenderer>("Reijo", 1, 0, "WhittedRenderer");
    qmlRegisterType<HdrViewer>("Reijo", 1, 0, "HdrViewer");
    qmlRegisterType<Renderer>();
    qmlRegisterType<Light>();
    qmlRegisterType<PointLight>("Reijo", 1, 0, "PointLight");
    qmlRegisterType<Material>("Reijo", 1, 0, "Material");
    qmlRegisterType<Plane>("Reijo", 1, 0, "Plane");

    QQmlEngine engine;
    QQmlComponent qmlComponent(&engine, "qml/testscene.qml");
    if (qmlComponent.isError()) {
        qDebug() << qmlComponent.errorString();
        return 0;
    }
    QObject* o = qmlComponent.create();

    Scene* scene = qobject_cast<Scene*>(o);
    if (!scene) {
        qCritical() << "Root item of the qml file was not a Scene, but a" << o->metaObject()->className();
        delete o;
        return 0;
    }

//    scene->dumpNodes();
    scene->render();

    return a.exec();

}
