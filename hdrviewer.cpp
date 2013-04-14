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

#include <QFileDialog>
#include <QDebug>

HdrViewer::HdrViewer(QObject *parent) :
    QObject(parent),
    m_gamma(2.2f),
    m_exposure(1.0),
    m_buffer(nullptr),
    m_renderedImage(nullptr)
{
    m_settingsUi = new HdrViewerSettings();
    connect(m_settingsUi, &HdrViewerSettings::gammaChanged, this, &HdrViewer::render);
    connect(m_settingsUi, &HdrViewerSettings::exposureChanged, this, &HdrViewer::render);
    connect(m_settingsUi, &HdrViewerSettings::save, this, &HdrViewer::onSave);
}

HdrViewer::~HdrViewer()
{
    delete m_renderedImage;
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
    m_settingsUi->setGamma(m_gamma);
    m_settingsUi->setExposure(m_exposure);
}

void HdrViewer::onProgressiveUpdate()
{
}

void HdrViewer::onFrameComplete()
{
    if (!m_renderer) {
        qCritical() << "HdrViewer has no renderer as source";
        return;
    }

    int w = m_renderer->renderedWidth();
    int h = m_renderer->renderedHeight();

    if (m_renderedImage)
        delete m_renderedImage;
    m_renderedImage = new QImage(w, h, QImage::Format_ARGB32);
    m_renderer->copyRenderBuffer(m_buffer);
    render();

    m_viewWidget.setWindowTitle("Reijo HDR viewer");
    m_viewWidget.resize(w, h);
    m_viewWidget.move(300, 200);
    m_viewWidget.show();

    m_settingsUi->move(m_viewWidget.x(), m_viewWidget.y() + m_viewWidget.height() + 50);
    m_settingsUi->show();
}

void HdrViewer::render()
{
    if (!m_buffer)
        return;

    int w = m_renderer->renderedWidth();
    int h = m_renderer->renderedHeight();

    float gamma = m_settingsUi->gamma();
    float exposure = m_settingsUi->exposure();
    float *raw = m_buffer;

    int* bits = (int*) m_renderedImage->bits();
    for (int i=0; i < w * h; i++) {
        int r = qMin((float)pow(*raw * exposure, 1.0f / gamma) * 255.0f, 255.0f); raw++;
        int g = qMin((float)pow(*raw * exposure, 1.0f / gamma) * 255.0f, 255.0f); raw++;
        int b = qMin((float)pow(*raw * exposure, 1.0f / gamma) * 255.0f, 255.0f); raw++;
        *bits = qRgba(r, g, b, 255.0f); bits++;
    }

    m_viewWidget.setPixmap(QPixmap::fromImage(*m_renderedImage));
}

void HdrViewer::onSave()
{
    if (!m_renderedImage)
        return;

    QString filename = QFileDialog::getSaveFileName(&m_viewWidget, "Save as");
    if (filename.isEmpty())
        return;

    qDebug() << "Saving to" << filename;
    m_renderedImage->save(filename);
}
