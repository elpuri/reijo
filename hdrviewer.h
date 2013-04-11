#ifndef HDRVIEWER_H
#define HDRVIEWER_H

#include <QObject>
#include <QLabel>
#include <QtQml>

#include "proputils.h"

class Renderer;


class HdrViewer : public QObject,
                  public QQmlParserStatus
{
    Q_OBJECT

    SIMPLE_PROPERTY_DECL(Renderer*, renderer)

public:
    explicit HdrViewer(QObject *parent = 0);
    
protected:
    void classBegin();
    void componentComplete();

public slots:

    void onFrameComplete();
    void onProgressiveUpdate();

private:

private:
    Renderer* m_renderer;
    QLabel m_viewWidget;
};

#endif // HDRVIEWER_H
