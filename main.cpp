#include <QApplication>
#include <QDebug>
#include <QtQml>

#include "testobject.h"
#include "objecteditorwidget.h"

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

    scene->dumpNodes();
    scene->render();

    /*
    TestObject o;
    ObjectEditorWidget editor(&o);
    editor.show();
*/

    QVector4D i(-0.5, 1.0, 1.0, 0.0);
    QVector4D n(0.0, 1.0, 0.0, 0.0);
    QVector4D reflected = MathUtils::reflect(-i, n);
    qDebug() << i << n << reflected;

    return a.exec();

}
