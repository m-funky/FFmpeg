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

#include "avcodec.h"
#include "internal.h"
#include "mediacodecenc.h"
#include "mediacodec_wrapper.h"

#define CODEC_MIME "video/avc"
#define COLOR_FORMAT 0x13 // YUV420Planar

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
    int ret = 0;

    FFAMediaFormat *format = NULL;
    MediaCodecH264EncContext *s = avctx->priv_data;

    format = ff_AMediaFormat_new();
    if (!format) {
        av_log(avctx, AV_LOG_ERROR, "Failed to create media format\n");
        ret = AVERROR_EXTERNAL;
        goto done;
    }

    ff_AMediaFormat_setString(format, "mime", CODEC_MIME);
    ff_AMediaFormat_setInt32(format, "width", avctx->width);
    ff_AMediaFormat_setInt32(format, "height", avctx->height);
    // TODO get color foramt from avctx->pix_fmt
    ff_AMediaFormat_setInt32(format, "color-format", COLOR_FORMAT);
    ff_AMediaFormat_setInt32(format, "bitrate", avctx->bit_rate);
    ff_AMediaFormat_setInt32(format, "frame-rate", 1 / av_q2d(avctx->time_base));
    ff_AMediaFormat_setInt32(format, "i-frame-interval", 10); // temporary

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
    .capabilities   = AV_CODEC_CAP_AUTO_THREADS,
    .pix_fmts       = (const enum AVPixelFormat[]){ AV_PIX_FMT_YUV420P,
                                                    AV_PIX_FMT_NONE },
};
