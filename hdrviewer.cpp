#include "hdrviewer.h"
#include "renderer.h"

HdrViewer::HdrViewer(QObject *parent) :
    QObject(parent)
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
        int r = qMin(*raw * 255, 255.0f); raw++;
        int g = qMin(*raw * 255, 255.0f); raw++;
        int b = qMin(*raw * 255, 255.0f); raw++;
        *bits = qRgba(r, g, b, 255.0f); bits++;
    }

    m_viewWidget.resize(w, h);
    m_viewWidget.setPixmap(QPixmap::fromImage(target));
    m_viewWidget.show();
}
