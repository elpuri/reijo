#-------------------------------------------------
#
# Project created by QtCreator 2013-03-30T13:41:10
#
#-------------------------------------------------


QT       += core qml widgets gui

TARGET = reijo
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x -O3
LIBS += -L /usr/local/lib -lavcodec -lavformat -lswscale -lavutil


SOURCES += src/main.cpp \
    src/objecteditorwidget.cpp \
    src/scene.cpp \
    src/camera.cpp \
    src/shapefactory.cpp \
    src/scenenode.cpp \
    src/shape.cpp \
    src/sphere.cpp \
    src/ray.cpp \
    src/whittedrenderer.cpp \
    src/light.cpp \
    src/pointlight.cpp \
    src/renderer.cpp \
    src/hdrviewer.cpp \
    src/mathutils.cpp \
    src/material.cpp \
    src/plane.cpp \
    src/hdrviewersettings.cpp \
    src/videoencoder.cpp \
    src/torus.cpp \
    src/disc.cpp \
    src/box.cpp \
    src/cylinder.cpp \
    src/regulargrid.cpp \
    src/boundingbox.cpp \
    src/imagewidget.cpp \
    src/cone.cpp

HEADERS += \
    src/proputils.h \
    src/objecteditorwidget.h \
    src/scene.h \
    src/camera.h \
    src/shapefactory.h \
    src/scenenode.h \
    src/shape.h \
    src/sphere.h \
    src/ray.h \
    src/whittedrenderer.h \
    src/renderer.h \
    src/light.h \
    src/pointlight.h \
    src/hdrviewer.h \
    src/mathutils.h \
    src/material.h \
    src/plane.h \
    src/hdrviewersettings.h \
    src/videoencoder.h \
    src/torus.h \
    src/disc.h \
    src/box.h \
    src/cylinder.h \
    src/regulargrid.h \
    src/boundingbox.h \
    src/imagewidget.h \
    src/cone.h

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
    qml/Sphereflake.qml \
    qml/conetestscene.qml \
    qml/glassgridscene.qml

FORMS += \
    hdrviewersettings.ui
