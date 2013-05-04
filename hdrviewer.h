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

#ifndef HDRVIEWER_H
#define HDRVIEWER_H

#include <QObject>
#include <QtQml>

#include "proputils.h"
#include "hdrviewersettings.h"
#include "imagewidget.h"

class Renderer;


class HdrViewer : public QObject,
                  public QQmlParserStatus
{
    Q_OBJECT

    SIMPLE_PROPERTY_DECL(Renderer*, renderer)
    SIMPLE_PROPERTY(float, gamma)
    SIMPLE_PROPERTY(float, exposure)

public:
    explicit HdrViewer(QObject *parent = 0);
    ~HdrViewer();
    
protected:
    void classBegin();
    void componentComplete();

signals:
    void clicked(int x, int y);

public slots:

    void onFrameComplete();
    void onProgressiveUpdate();
    void onSave();
    void render();

private:
    Renderer* m_renderer;
    ImageWidget m_viewWidget;
    float* m_buffer;
    HdrViewerSettings* m_settingsUi;
    QImage* m_renderedImage;
};

#endif // HDRVIEWER_H
