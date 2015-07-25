/*
 * This is a modified file to keep only parts required by the LZO decompressor
 *
 * The license for this file is the same than the one used by the FFmpeg project
 */

#ifndef AVUTIL_MEM_H
#define AVUTIL_MEM_H

/**
 * deliberately overlapping memcpy implementation
 * @param dst destination buffer
 * @param back how many bytes back we start (the initial size of the overlapping window), must be > 0
 * @param cnt number of bytes to copy, must be >= 0
 *
 * cnt > back is valid, this will copy the bytes we just copied,
 * thus creating a repeating pattern with a period length of back.
 */
void av_memcpy_backptr(uint8_t *dst, int back, int cnt);

#endif /* AVUTIL_MEM_H */