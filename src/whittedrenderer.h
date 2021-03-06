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

#ifndef WHITTEDRENDERER_H
#define WHITTEDRENDERER_H

#include "proputils.h"
#include "renderer.h"

#include <QtQml>
#include <QString>
#include <QVector3D>
#include <QRunnable>

class Ray;
class Shape;
class Light;
class Camera;
class RegularGrid;

class WhittedRenderer : public Renderer,
                        public QQmlParserStatus
{
    Q_OBJECT

public:
    enum AntiAliasingMode { None, JitteredSamples };
    Q_ENUMS(AntiAliasingMode)

    SIMPLE_PROPERTY(QVector3D, ambientLightColor)
    SIMPLE_PROPERTY(int, maxRecursionDepth)
    SIMPLE_PROPERTY(bool, coloredShadows)
    SIMPLE_PROPERTY(AntiAliasingMode, antiAliasing)
    SIMPLE_PROPERTY(int, samplesPerPixel)

    explicit WhittedRenderer(QObject *parent = 0);


public:
    void render(Scene* scene);
    void copyRenderBuffer(float *&buffer);

public slots:
    void onPicked(const int x, const int y);

protected:
    void classBegin();
    void componentComplete();

private:
    QVector3D trace(const Ray& r, int depth, const QList<Shape*>& shapes, const QList<Light*>& lights);
    void renderScanline(int y);


private:
    float* m_buffer;
    int m_hwThreadCount;

    // Common state for rendering threads
    Camera* m_activeCamera;
    QList<Shape*> m_shapes;
    QList<Light*> m_lights;

    RegularGrid* m_grid;

    friend class WhittedRunnable;

    class WhittedRunnable : public QRunnable {

    public:
        explicit WhittedRunnable(WhittedRenderer* r, int interleave, int interleaveCount);
        void run();

        WhittedRenderer* m_renderer;
        int m_interleave;
        int m_interleaveCount;
    };

};

#endif // WHITTEDRENDERER_H
