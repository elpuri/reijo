#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <QObject>
#include <QtQml>
#include <QString>
#include "proputils.h"

extern "C" {
#define __STDC_CONSTANT_MACROS
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavcodec/opt.h"
#include "libavutil/rational.h"
#include "libavutil/avstring.h"
#include "libswscale/swscale.h"
}

class Renderer;

class VideoEncoder : public QObject,
                     public QQmlParserStatus
{
    Q_OBJECT

    SIMPLE_PROPERTY(Renderer*, renderer)
    SIMPLE_PROPERTY(QString, filename)
    SIMPLE_PROPERTY(int, gop)
    SIMPLE_PROPERTY(int, bitrate)
    SIMPLE_PROPERTY(int, fps)

public:
    explicit VideoEncoder(QObject *parent = 0);
    
protected:
    void classBegin();
    void componentComplete();

signals:
    
public slots:
    void onRenderingStarted();
    void onFrameFromRenderer();
    void onRenderingCompleted();

private:
    int m_width;
    int m_height;

    bool m_initOk;

    AVOutputFormat* m_outputFormat;
    AVFormatContext* m_formatContext;
    AVCodecContext* m_codecContext;
    AVCodec* m_codec;
    AVStream* m_videoStream;
    AVFrame* m_frame;
    uint8_t* m_yuvBuffer;
    int m_outputBufferSize;
    uint8_t* m_outputBuffer;
    SwsContext* m_swsContext;
};

#endif // VIDEOENCODER_H
