/* btk_gzflags.c -- function extracted from gzwrite.c
 * Copyright (C) 2012 Arnaud Barré (function written by Mark Adler)
 * For conditions of distribution and use, see copyright notice in zlib.h
 */
 
#include "zlib.h"

/* used by zlibVersion() to get the vsnprintf story from the horse's mouth */
unsigned long ZEXPORT gzflags()
{
    unsigned long flags = 0;
#if defined(STDC) || defined(Z_HAVE_STDARG_H)
#  ifdef NO_vsnprintf
    flags += 1L << 25;
#    ifdef HAS_vsprintf_void
    flags += 1L << 26;
#    endif
#  else
#    ifdef HAS_vsnprintf_void
    flags += 1L << 26;
#    endif
#  endif
#else
    flags += 1L << 24;
#  ifdef NO_snprintf
    flags += 1L << 25;
#    ifdef HAS_sprintf_void
    flags += 1L << 26;
#    endif
#  else
#    ifdef HAS_snprintf_void
    flags += 1L << 26;
#    endif
#  endif
#endif
    return flags;
}
