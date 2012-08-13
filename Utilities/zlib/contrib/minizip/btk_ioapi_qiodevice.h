/* 
 * Copyright (C) 2012 Arnaud Barré
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef __btk_ioapi_qiodevice_h
#define __btk_ioapi_qiodevice_h

#include "ioapi.h"

void fill_qiodevice_open_filefunc OF((zlib_filefunc_def* pzlib_filefunc_def));
void fill_qiodevice_open64_filefunc OF((zlib_filefunc64_def* pzlib_filefunc_def));

#endif // __btk_ioapi_qiodevice_h