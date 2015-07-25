/*
 * This is a modified file to keep only the required part for the LZO decompressor
 *
 * The license for this file is the same than the one used by the FFmpeg project
 */

#ifndef AVUTIL_INTREADWRITE_H
#define AVUTIL_INTREADWRITE_H

#include "config.h" // HAVE_BIGENDIAN

#include <stdint.h>

#if HAVE_BIGENDIAN

// ------------------------------------------------------------------------- //
//                                 BIG ENDIAN
// ------------------------------------------------------------------------- // 

// Read big endian 16-bits
#define AV_RB16(x)                   \
  ((((const uint8_t*)(x))[0] << 8) | ((const uint8_t*)(x))[1])

// Write big endian 16-bits
#define AV_WB16(p, darg) do {    \
    unsigned d = (darg);         \
    ((uint8_t*)(p))[1] = (d);    \
    ((uint8_t*)(p))[0] = (d)>>8; \
  } while(0)

// Read big endian 24-bits
#define AV_RB24(x)                    \
  ((((const uint8_t*)(x))[0] << 16) | \
   (((const uint8_t*)(x))[1] <<  8) | \
    ((const uint8_t*)(x))[2])

// Read big endian 32-bits
#define AV_RB32(x)                              \
  (((uint32_t)((const uint8_t*)(x))[0] << 24) | \
             (((const uint8_t*)(x))[1] << 16) | \
             (((const uint8_t*)(x))[2] <<  8) | \
              ((const uint8_t*)(x))[3])

// Write big endian 32-bits
#define AV_WB32(p, darg) do {     \
    unsigned d = (darg);          \
    ((uint8_t*)(p))[3] = (d);     \
    ((uint8_t*)(p))[2] = (d)>>8;  \
    ((uint8_t*)(p))[1] = (d)>>16; \
    ((uint8_t*)(p))[0] = (d)>>24; \
  } while(0)

// Map big endian macros to native endian macros
#define AV_RN16(x) AV_RB16(x)
#define AV_WN16(p, v) AV_WB16(p, v)
#define AV_RN32(x) AV_RB32(x)
#define AV_WN32(p, v) AV_WB32(p, v)

#else

// ------------------------------------------------------------------------- //
//                                LITTLE ENDIAN
// ------------------------------------------------------------------------- //

// Read little endian 16-bits
#define AV_RL16(x)                   \
  ((((const uint8_t*)(x))[1] << 8) | \
    ((const uint8_t*)(x))[0])

// Write little endian 16-bits
#define AV_WL16(p, darg) do {    \
    unsigned d = (darg);         \
    ((uint8_t*)(p))[0] = (d);    \
    ((uint8_t*)(p))[1] = (d)>>8; \
  } while(0)

// Read little endian 24-bits
#define AV_RL24(x)                    \
  ((((const uint8_t*)(x))[2] << 16) | \
   (((const uint8_t*)(x))[1] <<  8) | \
  ((const uint8_t*)(x))[0])

// Read little endian 32-bits
#define AV_RL32(x)                              \
  (((uint32_t)((const uint8_t*)(x))[3] << 24) | \
         (((const uint8_t*)(x))[2] << 16) |     \
         (((const uint8_t*)(x))[1] <<  8) |     \
        ((const uint8_t*)(x))[0])

// Write little endian 32-bits
#define AV_WL32(p, darg) do {     \
    unsigned d = (darg);          \
    ((uint8_t*)(p))[0] = (d);     \
    ((uint8_t*)(p))[1] = (d)>>8;  \
    ((uint8_t*)(p))[2] = (d)>>16; \
    ((uint8_t*)(p))[3] = (d)>>24; \
  } while(0)

// Map little endian macros to native endian macros
#define AV_RN16(x) AV_RL16(x)
#define AV_WN16(p, v) AV_WL16(p, v)
#define AV_RN32(x) AV_RL32(x)
#define AV_WN32(p, v) AV_WL32(p, v)

#endif // HAVE_BIG_ENDIAN

// ------------------------------------------------------------------------- //
//                                NATIVE ENDIAN
// ------------------------------------------------------------------------- //

// Copy (native endian) 16-bits unaligned  
#define AV_COPY16U(d,s) AV_WN16(d, AV_RN16(s))

// Copy (native endian) 32-bits unaligned  
#define AV_COPY32U(d,s) AV_WN32(d, AV_RN32(s))

#endif /* AVUTIL_INTREADWRITE_H */