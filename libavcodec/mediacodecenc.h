/*
 * Android MediaCodec encoder
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

#ifndef AVCODEC_MEDIACODECENC_H
#define AVCODEC_MEDIACODECENC_H

#include <stdint.h>
#include <sys/types.h>

#include "libavutil/frame.h"
#include "libavutil/pixfmt.h"

#include "avcodec.h"
#include "mediacodec_wrapper.h"

typedef struct MediaCodecEncContext {

    FFAMediaCodec *codec;
    FFAMediaFormat *format;

    int flushing;

    int width;
    int height;
    int color_format;
    enum AVPixelFormat pix_fmt;

    int queued_buffer_nb;
    int queued_buffer_max;
    uint64_t dequeued_buffer_nb;

    uint8_t *extradata;
    int extradata_size;

    int first_buffer;

} MediaCodecEncContext;

int ff_mediacodec_enc_init(AVCodecContext *avctx,
                           MediaCodecEncContext *s,
                           const char *mime,
                           FFAMediaFormat *format);

int ff_mediacodec_enc_encode(AVCodecContext *avctx,
                             MediaCodecEncContext *s,
                             AVPacket *pkt,
                             AVFrame *frame,
                             int *got_packet,
                             int frame_size);

int ff_mediacodec_enc_close(AVCodecContext *avctx,
                            MediaCodecEncContext *s);

#endif /* AVCODEC_MEDIACODECENC_H */
