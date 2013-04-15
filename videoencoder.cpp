#include "videoencoder.h"
#include "renderer.h"


#define checkPtr(ptr)\
    if (!ptr) {\
    qDebug() << "VideoDecoder: Couldn't alloc " #ptr "";\
    return;}

VideoEncoder::VideoEncoder(QObject *parent) :
    QObject(parent),
    m_renderer(nullptr),
    m_initOk(false),
    m_bitrate(500000),
    m_gop(30),
    m_fps(30),
    m_swsContext(nullptr)
{
    avcodec_register_all();
    av_register_all();

    qDebug() << "Available h264 codecs:";
    AVCodec* codec = av_codec_next(nullptr);
    while (codec) {

        if (codec->id == AV_CODEC_ID_H264)
            qDebug() << codec->name << codec->id;
        codec = av_codec_next(codec);
    }
}


void VideoEncoder::classBegin()
{

}

void VideoEncoder::componentComplete()
{
    if (!m_renderer) {
        qCritical() << "VideoEncoder: No renderer assigned";
        return;
    }
    connect(m_renderer, &Renderer::renderingStarted, this, &VideoEncoder::onRenderingStarted);
    connect(m_renderer, &Renderer::renderingComplete, this, &VideoEncoder::onRenderingCompleted);
    connect(m_renderer, &Renderer::frameComplete, this, &VideoEncoder::onFrameFromRenderer);
    m_width = m_renderer->renderedWidth();
    m_height = m_renderer->renderedHeight();

}

void VideoEncoder::onRenderingStarted()
{
    m_outputFormat = av_guess_format(NULL, m_filename.toStdString().c_str(), NULL);
    if (!m_outputFormat) {
        qDebug() << "VideoEncoder: Couldn't guess format from name, defaulting to mpeg";
        m_outputFormat = av_guess_format("mpeg", NULL, NULL);
    }

    m_outputFormat->video_codec = AV_CODEC_ID_H264;
    m_formatContext = avformat_alloc_context();
    checkPtr(m_formatContext);

    m_formatContext->oformat = m_outputFormat;
    snprintf(m_formatContext->filename, sizeof(m_formatContext), "%s", m_filename.toStdString().c_str());

    m_videoStream = avformat_new_stream(m_formatContext, NULL);
    checkPtr(m_videoStream);
    m_videoStream->id = 0;

    m_codecContext = m_videoStream->codec;
    checkPtr(m_codecContext);

    m_codecContext->codec_id = m_outputFormat->video_codec;
    m_codecContext->thread_count = 8;
    m_codecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    m_codecContext->width = m_width;
    m_codecContext->height = m_height;
    m_codecContext->bit_rate = m_bitrate;
    m_codecContext->rc_max_rate = 0;
    m_codecContext->rc_buffer_size = 0;
    m_codecContext->gop_size = m_gop;
    m_codecContext->max_b_frames = 3;
    m_codecContext->b_frame_strategy = 1;
    m_codecContext->coder_type = 1;
    m_codecContext->me_cmp = 1;
    m_codecContext->me_range = 16;
    m_codecContext->qmin = 10;
    m_codecContext->qmax = 51;
    m_codecContext->scenechange_threshold = 40;
    m_codecContext->flags |= CODEC_FLAG_LOOP_FILTER;
    m_codecContext->me_method = ME_HEX;
    m_codecContext->i_quant_factor = 0.71;
    m_codecContext->qcompress = 0.6;
    m_codecContext->max_qdiff = 4;
    m_codecContext->pix_fmt = PIX_FMT_YUV420P;
    m_codecContext->time_base = (AVRational) { 1, 30 };

    m_codecContext->coder_type = 1;  // coder = 1
    m_codecContext->me_subpel_quality = 7;   // subq=7
    m_codecContext->me_range = 16;   // me_range=16
    m_codecContext->keyint_min = 25; // keyint_min=25
    m_codecContext->i_quant_factor = 0.71; // i_qfactor=0.71
    m_codecContext->b_frame_strategy = 1;  // b_strategy=1
    m_codecContext->qcompress = 0.6; // qcomp=0.6
    m_codecContext->qmin = 10;   // qmin=10
    m_codecContext->qmax = 51;   // qmax=51
    m_codecContext->max_qdiff = 4;   // qdiff=4
    m_codecContext->max_b_frames = 3;    // bf=3
    m_codecContext->refs = 3;    // refs=3
    m_codecContext->trellis = 1; // trellis=1

    // libx264-main.ffpreset preset

    if (m_formatContext->oformat->flags & AVFMT_GLOBALHEADER)
        m_codecContext->flags |= CODEC_FLAG_GLOBAL_HEADER;

    qDebug() << "Getting codec with name libx264";
    m_codec = avcodec_find_encoder_by_name("libx264");
    checkPtr(m_codec);
    if (avcodec_open2(m_codecContext, m_codec, NULL) < 0) {
        qDebug() << "VideoEncoder: Couldn't open codec";
        return;
    }

    m_frame = avcodec_alloc_frame();
    m_frame->pts = 0;
    checkPtr(m_frame);

    int yuvSize = avpicture_get_size(m_codecContext->pix_fmt, m_codecContext->width, m_codecContext->height);
    m_yuvBuffer = new uint8_t[yuvSize];

    // Setup the planes
    avpicture_fill((AVPicture *)m_frame, m_yuvBuffer, m_codecContext->pix_fmt, m_codecContext->width, m_codecContext->height);
    if (avio_open(&m_formatContext->pb, m_filename.toStdString().c_str(), AVIO_FLAG_WRITE) < 0) {
        qDebug() << "VideoEncoder: Couldn't open" << m_filename << "for writing";
        return;
    }

    avformat_write_header(m_formatContext, NULL);

    m_outputBufferSize = m_width * m_height * 3;    // This has nothing to do with the original frame size, but i guess this has to be enough...
    m_outputBuffer = new uint8_t[m_outputBufferSize];

    m_initOk = true;
}

void VideoEncoder::onFrameFromRenderer()
{
    QElapsedTimer elapsed;
    elapsed.start();
    if (!m_initOk)
        return;

    float* rawFrame;
    m_renderer->copyRenderBuffer(rawFrame);
    float* rawPtr = rawFrame;

    // TODO: extract tonemapping algorithm from HDRViewer and run here
    uint8_t* rgbBuffer = new uint8_t[m_width * m_height * 3];
    uint8_t* rgbPtr = rgbBuffer;
    for (int i=0; i < m_width * m_height; i++) {

        *rgbPtr++ = qMin((float) pow(*rawPtr++, 1.0f / 1.9f) * 255.0f, 255.0f);
        *rgbPtr++ = qMin((float) pow(*rawPtr++, 1.0f / 1.9f) * 255.0f, 255.0f);
        *rgbPtr++ = qMin((float) pow(*rawPtr++, 1.0f / 1.9f) * 255.0f, 255.0f);
    }
    delete rawFrame;

    m_swsContext = sws_getCachedContext(m_swsContext, m_width, m_height, PIX_FMT_RGB24, m_width, m_height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    if (!m_swsContext) {
        qDebug() << "VideoDecoder: Couldn't get sws context";
        delete rgbBuffer;
        return;
    }

    uint8_t *sourcePlanes[3];
    sourcePlanes[0] = rgbBuffer;
    sourcePlanes[1] = 0;
    sourcePlanes[2] = 0;

    int sourceStride[3];
    sourceStride[0] = m_width * 3;
    sourceStride[1] = 0;
    sourceStride[2] = 0;

    int sliceH = sws_scale(m_swsContext, sourcePlanes, sourceStride, 0, m_height, m_frame->data, m_frame->linesize);
    int outSize = avcodec_encode_video(m_codecContext, m_outputBuffer, m_outputBufferSize, m_frame);
    if (outSize > 0) {
        AVPacket pkt;
        av_init_packet(&pkt);
        if (m_codecContext->coded_frame->pts !=(0x8000000000000000LL))
            pkt.pts = av_rescale_q(m_codecContext->coded_frame->pts, m_codecContext->time_base, m_videoStream->time_base);
        if (m_codecContext->coded_frame->key_frame)
            pkt.flags |= AV_PKT_FLAG_KEY;
        pkt.stream_index = m_videoStream->index;
        pkt.data = m_outputBuffer;
        pkt.size = outSize;
        if (av_interleaved_write_frame(m_formatContext, &pkt) < 0) {
            qDebug() << "VideoEncoder: Couldn't write frame";
            return;
        }
    }
    delete rgbBuffer;
    qDebug() << "VideoEncoder:: encoded frame number" << m_frame->pts << "in" << elapsed.elapsed() << "ms";

    m_frame->pts++;
}

void VideoEncoder::onRenderingCompleted()
{
    if (!m_initOk)
        return;
    qDebug() << "VideoEncoder: Rendering complete, closing shop";
    av_write_trailer(m_formatContext);
    avcodec_close(m_videoStream->codec);

    for(int i = 0; i < m_formatContext->nb_streams; i++) {
       av_freep(&m_formatContext->streams[i]->codec);
       av_freep(&m_formatContext->streams[i]);
    }

    avio_close(m_formatContext->pb);
    av_free(m_formatContext);
}
