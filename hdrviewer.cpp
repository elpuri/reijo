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

#include "hdrviewer.h"
#include "renderer.h"

HdrViewer::HdrViewer(QObject *parent) :
    QObject(parent),
    m_gamma(2.2f)
{

}

Renderer* HdrViewer::renderer()
{
    return m_renderer;
}

void HdrViewer::rendererSetter(Renderer* renderer)
{
    qDebug() << Q_FUNC_INFO;
    m_renderer = renderer;
    disconnect(this, SLOT(onFrameComplete()));
    disconnect(this, SLOT(onProgressiveUpdate()));

    connect(m_renderer, &Renderer::progressiveUpdate, this, &HdrViewer::onProgressiveUpdate);
    connect(m_renderer, &Renderer::frameComplete, this, &HdrViewer::onFrameComplete);
}

void HdrViewer::classBegin()
{

}

void HdrViewer::componentComplete()
{

}

void HdrViewer::onProgressiveUpdate()
{

}

void HdrViewer::onFrameComplete()
{
    float *buffer;
    int w = m_renderer->renderedWidth();
    int h = m_renderer->renderedHeight();
    qDebug() << Q_FUNC_INFO << w << h;

    m_renderer->copyRenderBuffer(buffer);
    float *raw = buffer;

    QImage target(w, h, QImage::Format_ARGB32);
    int* bits = (int*) target.bits();
    for (int i=0; i < w * h; i++) {
        int r = qMin((float)pow(*raw, 1.0f / m_gamma) * 255.0f, 255.0f); raw++;
        int g = qMin((float)pow(*raw, 1.0f / m_gamma) * 255.0f, 255.0f); raw++;
        int b = qMin((float)pow(*raw, 1.0f / m_gamma) * 255.0f, 255.0f); raw++;
        *bits = qRgba(r, g, b, 255.0f); bits++;
    }

    m_viewWidget.resize(w, h);
    m_viewWidget.setPixmap(QPixmap::fromImage(target));
    m_viewWidget.show();
}
