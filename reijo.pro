#-------------------------------------------------
#
# Project created by QtCreator 2013-03-30T13:41:10
#
#-------------------------------------------------


QT       += core qml widgets gui

TARGET = reijo
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x -O3
LIBS += -L /usr/local/lib -lavcodec -lavformat -lswscale


SOURCES += main.cpp \
    objecteditorwidget.cpp \
    scene.cpp \
    camera.cpp \
    shapefactory.cpp \
    scenenode.cpp \
    shape.cpp \
    sphere.cpp \
    ray.cpp \
    whittedrenderer.cpp \
    light.cpp \
    pointlight.cpp \
    renderer.cpp \
    hdrviewer.cpp \
    mathutils.cpp \
    material.cpp \
    plane.cpp \
    hdrviewersettings.cpp \
    videoencoder.cpp \
    torus.cpp \
    disc.cpp \
    box.cpp \
    cylinder.cpp \
    regulargrid.cpp \
    boundingbox.cpp \
    imagewidget.cpp

HEADERS += \
    proputils.h \
    objecteditorwidget.h \
    scene.h \
    camera.h \
    shapefactory.h \
    scenenode.h \
    shape.h \
    sphere.h \
    ray.h \
    whittedrenderer.h \
    renderer.h \
    light.h \
    pointlight.h \
    hdrviewer.h \
    mathutils.h \
    material.h \
    plane.h \
    hdrviewersettings.h \
    videoencoder.h \
    torus.h \
    disc.h \
    box.h \
    cylinder.h \
    regulargrid.h \
    boundingbox.h \
    imagewidget.h

OTHER_FILES += \
    qml/testscene.qml \
    todo.txt \
    qml/Room.qml \
    .gitignore \
    qml/animscene.qml \
    README.md \
    qml/rainbowballs.qml \
    qml/torustest.qml \
    qml/boxtest.qml \
    qml/cubearray.qml \
    qml/MarkerSphere.qml \
    qml/MirrorBall.qml \
    qml/cylindertest.qml \
    qml/transparencytest.qml \
    qml/glassyanim.qml \
    qml/glasstorusanim.qml \
    qml/gridtestscene.qml \
    qml/sphereflakescene.qml \
    qml/Sphereflake.qml

FORMS += \
    hdrviewersettings.ui
