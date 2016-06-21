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

#include "avcodec.h"
#include "internal.h"
#include "mediacodecenc.h"
#include "mediacodec_wrapper.h"

typedef struct MediaCodecH264EncContext {
} MediaCodecH264EncContext;

static av_cold int mediacodec_encode_close(AVCodecContext *avctx)
{
    return 0;
}

static av_cold int mediacodec_encode_init(AVCodecContext *avctx)
{
    return 0;
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
