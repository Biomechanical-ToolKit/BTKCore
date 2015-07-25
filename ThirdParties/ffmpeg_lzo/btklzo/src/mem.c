/*
 * This is a modified file to keep only the required part for the LZO decompressor
 *
 * The license for this file is the same than the one used by the FFmpeg project
 */

#include "config.h" // HAVE_BIGENDIAN
#include "intreadwrite.h"
#include "mem.h"

static void fill16(uint8_t *dst, int len)
{
    uint32_t v = AV_RN16(dst - 2);

    v |= v << 16;

    while (len >= 4) {
        AV_WN32(dst, v);
        dst += 4;
        len -= 4;
    }

    while (len--) {
        *dst = dst[-2];
        dst++;
    }
}

static void fill24(uint8_t *dst, int len)
{
#if HAVE_BIGENDIAN
    uint32_t v = AV_RB24(dst - 3);
    uint32_t a = v << 8  | v >> 16;
    uint32_t b = v << 16 | v >> 8;
    uint32_t c = v << 24 | v;
#else
    uint32_t v = AV_RL24(dst - 3);
    uint32_t a = v       | v << 24;
    uint32_t b = v >> 8  | v << 16;
    uint32_t c = v >> 16 | v << 8;
#endif

    while (len >= 12) {
        AV_WN32(dst,     a);
        AV_WN32(dst + 4, b);
        AV_WN32(dst + 8, c);
        dst += 12;
        len -= 12;
    }

    if (len >= 4) {
        AV_WN32(dst, a);
        dst += 4;
        len -= 4;
    }

    if (len >= 4) {
        AV_WN32(dst, b);
        dst += 4;
        len -= 4;
    }

    while (len--) {
        *dst = dst[-3];
        dst++;
    }
}

static void fill32(uint8_t *dst, int len)
{
    uint32_t v = AV_RN32(dst - 4);

    while (len >= 4) {
        AV_WN32(dst, v);
        dst += 4;
        len -= 4;
    }

    while (len--) {
        *dst = dst[-4];
        dst++;
    }
}

void av_memcpy_backptr(uint8_t *dst, int back, int cnt)
{
    const uint8_t *src = &dst[-back];
    if (!back)
        return;

    if (back == 1) {
        memset(dst, *src, cnt);
    } else if (back == 2) {
        fill16(dst, cnt);
    } else if (back == 3) {
        fill24(dst, cnt);
    } else if (back == 4) {
        fill32(dst, cnt);
    } else {
        if (cnt >= 16) {
            int blocklen = back;
            while (cnt > blocklen) {
                memcpy(dst, src, blocklen);
                dst       += blocklen;
                cnt       -= blocklen;
                blocklen <<= 1;
            }
            memcpy(dst, src, cnt);
            return;
        }
        if (cnt >= 8) {
            AV_COPY32U(dst,     src);
            AV_COPY32U(dst + 4, src + 4);
            src += 8;
            dst += 8;
            cnt -= 8;
        }
        if (cnt >= 4) {
            AV_COPY32U(dst, src);
            src += 4;
            dst += 4;
            cnt -= 4;
        }
        if (cnt >= 2) {
            AV_COPY16U(dst, src);
            src += 2;
            dst += 2;
            cnt -= 2;
        }
        if (cnt)
            *dst = *src;
    }
}