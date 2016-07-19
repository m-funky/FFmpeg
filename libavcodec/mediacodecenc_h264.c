/*
 * Android MediaCodec H.264 encoder
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "libavutil/fifo.h"
#include "libavutil/pixdesc.h"

#include "avcodec.h"
#include "internal.h"
#include "mediacodecenc.h"
#include "mediacodec_wrapper.h"

#define CODEC_MIME "video/avc"

static int frame_num = 0;
static int packet_num = 0;

typedef struct MediaCodecH264EncContext {

    MediaCodecEncContext ctx;

    AVFifoBuffer *fifo;

} MediaCodecH264EncContext;

static av_cold int mediacodec_encode_close(AVCodecContext *avctx)
{
    MediaCodecH264EncContext *s = avctx->priv_data;

    ff_mediacodec_enc_close(avctx, &s->ctx);

    av_fifo_free(s->fifo);

    return 0;
}

static av_cold int mediacodec_encode_init(AVCodecContext *avctx)
{
    frame_num = 0;
    packet_num = 0;

    int ret = 0;

    FFAMediaFormat *format = NULL;
    MediaCodecH264EncContext *s = avctx->priv_data;
    MediaCodecEncContext *ctx = &s->ctx;

    format = ff_AMediaFormat_new();
    if (!format) {
        av_log(avctx, AV_LOG_ERROR, "Failed to create media format\n");
        ret = AVERROR_EXTERNAL;
        goto done;
    }

    ctx->width = avctx->width;
    ctx->height = avctx->height;
    ctx->color_format = ff_mediacodec_select_color_format(avctx, CODEC_MIME);
    ctx->pix_fmt = avctx->pix_fmt;
    ctx->bit_rate = avctx->bit_rate;

    av_log(avctx, AV_LOG_INFO, "MediaCodec encoder color format=%d  \n", ctx->color_format);

    ff_AMediaFormat_setString(format, "mime", CODEC_MIME);
    ff_AMediaFormat_setInt32(format, "width", ctx->width);
    ff_AMediaFormat_setInt32(format, "height", ctx->height);
    ff_AMediaFormat_setInt32(format, "color-format", ctx->color_format);
    ff_AMediaFormat_setInt32(format, "bitrate", ctx->bit_rate);
    ff_AMediaFormat_setInt32(format, "frame-rate", 1 / av_q2d(avctx->time_base));
    ff_AMediaFormat_setInt32(format, "i-frame-interval", 12); // same as openh264 default.

    if ((ret = ff_mediacodec_enc_init(avctx, &s->ctx, CODEC_MIME, format)) < 0) {
        goto done;
    }

    av_log(avctx, AV_LOG_INFO, "MediaCodec encoder started successfully, ret = %d\n", ret);

    s->fifo = av_fifo_alloc(sizeof(AVPacket));
    if (!s->fifo) {
        ret = AVERROR(ENOMEM);
        goto done;
    }

done:
    if (format) {
        ff_AMediaFormat_delete(format);
    }

    if (ret < 0) {
        mediacodec_encode_close(avctx);
    }
    return ret;
}

static av_cold int mediacodec_encode_frame(AVCodecContext *avctx, AVPacket *avpkt,
    const AVFrame *frame, int *got_packet)
{
    frame_num++;

    if (frame) {
        int64_t pts_us = frame->pts * av_q2d(avctx->time_base) * 1000 * 1000;
        av_log(avctx, AV_LOG_DEBUG,
                "[e][log][F] %d format=%s key_frame=%d linesizes=(%d,%d,%d) pts=%"PRIi64" ..\n",
                frame_num, av_get_pix_fmt_name(avctx->pix_fmt), frame->key_frame,
                frame->linesize[0], frame->linesize[1], frame->linesize[2],
                frame->pts);
    } else {
        av_log(avctx, AV_LOG_DEBUG,
                "[e][log][F] %d format=%s ..\n",
                frame_num, av_get_pix_fmt_name(avctx->pix_fmt));
    }

    MediaCodecH264EncContext *s = avctx->priv_data;
    MediaCodecEncContext *ctx = &s->ctx;

    int ret = 0;

    // tmp for nv12

    int frame_size = ctx->width * ctx->height + ctx->width * ctx->height / 2;
    int offset = 0;

    if (!frame) {
        frame_size = 0;
    }


    /*
    for (i = 0; i < 2; i++) {
        int height;
        int width;

        if (i == 0) {
            height = 640;
            width = 640;
        } else {
            height = 640 / 2;
            width = 640;
        }

        memcpy(avpkt->data + offset, frame->data[i], height * width);
        av_log(avctx, AV_LOG_DEBUG,
                "[e][log][M] format=%s size=%d pts=%"PRId64" src= %"PRIu8" dst=%"PRIu8" ..\n" ,
                av_get_pix_fmt_name(avctx->pix_fmt),
                height * width, frame->pts, frame->data[i], avpkt->data + offset);

        offset += height * width;
    }
    */


    if (av_fifo_space(s->fifo) < sizeof(frame_size)) {
        ret = av_fifo_realloc2(s->fifo,
                av_fifo_size(s->fifo) + sizeof(frame_size));
        if (ret < 0) {
            av_log(avctx, AV_LOG_ERROR, "Error getting fifo.\n");
            return ret;
        }
    }

    //av_fifo_generic_write(s->fifo, frame->data[0], 640 * 640, NULL);
    //av_fifo_generic_write(s->fifo, frame->data[1], 640 * 640 / 2, NULL);

    /*
    av_log(avctx, AV_LOG_DEBUG,
            "[e][log][B] format=%s fifo.size=%d fifo.space=%d pts=%"PRId64" ..\n" ,
            av_get_pix_fmt_name(avctx->pix_fmt),
            av_fifo_size(s->fifo), av_fifo_space(s->fifo), frame->pts);
            */

    /*
    if ((ret = ff_alloc_packet2(avctx, avpkt, frame_size, frame_size))) {
        av_log(avctx, AV_LOG_ERROR, "Error getting output packet\n");
        return ret;
    } else {
        av_log(avctx, AV_LOG_DEBUG, "Success getting output packet\n");
    }
    */


    //memcpy(avpkt->data, frame->data[0], 640 * 640);
    //memcpy(avpkt->data + 640 * 640, frame->data[1], 640 * 640 / 2);

    //av_fifo_generic_read(s->fifo, avpkt->data, frame_size, NULL);

    while (!*got_packet) {
        if (offset >= frame_size) {
            if (frame_size == 0) {
                ff_mediacodec_enc_encode(avctx, ctx, avpkt, frame, got_packet, 0);
            }
            return 0;
        }

        ret = ff_mediacodec_enc_encode(avctx, ctx, avpkt, frame, got_packet, frame_size);

        if (ret < 0) {
            av_log(avctx, AV_LOG_ERROR, "Failed to encode. \n");
            return ret;
        }

        offset += ret;

        /*
        av_log(avctx, AV_LOG_DEBUG,
                "[e][log][A] format=%s packet=%d pts=%"PRId64" size=%d ..\n",
                av_get_pix_fmt_name(avctx->pix_fmt), *got_packet, frame->pts, offset);
                */

    }

    packet_num++;

    av_log(avctx, AV_LOG_DEBUG,
            "[e][log][P] %d format=%s size=%d pts=%"PRId64" dts=%"PRId64" flags=%d ..\n" ,
            packet_num, av_get_pix_fmt_name(avctx->pix_fmt),
            avpkt->size, avpkt->pts, avpkt->dts, avpkt->flags);

    return 0;
}

AVCodec ff_h264_mediacodec_encoder = {
    .name           = "h264_mediacodec",
    .long_name      = NULL_IF_CONFIG_SMALL("H.264 Android MediaCodec encoder"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_H264,
    .priv_data_size = sizeof(MediaCodecH264EncContext),
    .init           = mediacodec_encode_init,
    .encode2        = mediacodec_encode_frame,
    .close          = mediacodec_encode_close,
    .capabilities   = CODEC_CAP_DELAY,
    .pix_fmts       = (const enum AVPixelFormat[]){ AV_PIX_FMT_NV12,
                                                    AV_PIX_FMT_YUV420P,
                                                    AV_PIX_FMT_NONE },
};
