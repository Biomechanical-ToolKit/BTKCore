/* 
 * Copyright (C) 2012 Arnaud Barré
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
 * Note: This file was largerly inspired by the file qioapi.c (see QuaZIP project -- http://quazip.sourceforge.net/)
 *       as most of the code in each function is a copy of the original code.
 *       However, the version embedded with QuaZIP is a modified version of minizip
 *       and can not be adapted to the latest update (based on zilib 1.01e version 
 *       while the latest is 1.2.6). 
 */
 
#include "btk_ioapi_qiodevice.h"
#include "unzip.h"

#include <QIODevice>

static voidpf ZCALLBACK qiodevice_open64_file_func(voidpf /* opaque */, const void* filename, int mode)
{
  // Hugly casts but there is no other way to do it without modifying the source code of minizip...
  QIODevice* iodevice = const_cast<QIODevice*>(reinterpret_cast<const QIODevice*>(filename));
  
  if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) == ZLIB_FILEFUNC_MODE_READ)
      iodevice->open(QIODevice::ReadOnly);
  else if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
      iodevice->open(QIODevice::ReadWrite);
  else if (mode & ZLIB_FILEFUNC_MODE_CREATE)
      iodevice->open(QIODevice::WriteOnly);
  
  if (iodevice->isOpen())
  {
    if (iodevice->isSequential())
    {
      iodevice->close();
      return NULL;
    }
    else
    {
      return iodevice;
    }
  }
  else
  {
    return NULL;
  }
}

static voidpf ZCALLBACK qiodevice_open_file_func(voidpf opaque, const char* filename, int mode)
{
  // Hugly casts but there is no other way to do it without modifying the source code of minizip...
  return qiodevice_open64_file_func(opaque, reinterpret_cast<const void*>(filename), mode);
}

static uLong ZCALLBACK qiodevice_read_file_func(voidpf /* opaque */, voidpf stream, void* buf, uLong size)
{
  uLong ret;
  ret = (uLong)((QIODevice*)stream)->read((char*)buf,size);
  return ret;
}

static uLong ZCALLBACK qiodevice_write_file_func(voidpf /* opaque */, voidpf stream, const void* buf, uLong size)
{
  uLong ret;
  ret = (uLong)((QIODevice*)stream)->write((char*)buf,size);
  return ret;
}

static long ZCALLBACK qiodevice_tell_file_func(voidpf /* opaque */, voidpf stream)
{
  uLong ret;
  ret = ((QIODevice*)stream)->pos();
  return ret;
}


static ZPOS64_T ZCALLBACK qiodevice_tell64_file_func(voidpf /* opaque */, voidpf stream)
{
  ZPOS64_T ret;
  ret = ((QIODevice*)stream)->pos();
  return ret;
}

static long ZCALLBACK qiodevice_seek64_file_func(voidpf /* opaque */, voidpf stream, ZPOS64_T offset, int origin)
{
  uLong qiodevice_seek_result=0;
  int ret;
  switch (origin)
  {
  case ZLIB_FILEFUNC_SEEK_CUR :
      qiodevice_seek_result = ((QIODevice*)stream)->pos() + offset;
      break;
  case ZLIB_FILEFUNC_SEEK_END :
      qiodevice_seek_result = ((QIODevice*)stream)->size() - offset;
      break;
  case ZLIB_FILEFUNC_SEEK_SET :
      qiodevice_seek_result = offset;
      break;
  default: return -1;
  }
  ret = !((QIODevice*)stream)->seek(qiodevice_seek_result);
  return ret;
}

static long ZCALLBACK qiodevice_seek_file_func(voidpf opaque, voidpf stream, uLong offset, int origin)
{
  return qiodevice_seek64_file_func(opaque, stream, (ZPOS64_T)offset, origin);
}

static int ZCALLBACK qiodevice_close_file_func(voidpf /* opaque */, voidpf stream)
{
  ((QIODevice*)stream)->close();
  return UNZ_OK;
}

static int ZCALLBACK qiodevice_error_file_func(voidpf /* opaque */, voidpf stream)
{
  return !((QIODevice*)stream)->errorString().isEmpty();
}

void fill_qiodevice_open_filefunc(zlib_filefunc_def* pzlib_filefunc_def)
{
  pzlib_filefunc_def->zopen_file = qiodevice_open_file_func;
  pzlib_filefunc_def->zread_file = qiodevice_read_file_func;
  pzlib_filefunc_def->zwrite_file = qiodevice_write_file_func;
  pzlib_filefunc_def->ztell_file = qiodevice_tell_file_func;
  pzlib_filefunc_def->zseek_file = qiodevice_seek_file_func;
  pzlib_filefunc_def->zclose_file = qiodevice_close_file_func;
  pzlib_filefunc_def->zerror_file = qiodevice_error_file_func;
  pzlib_filefunc_def->opaque = NULL;
};

void fill_qiodevice_open64_filefunc(zlib_filefunc64_def*  pzlib_filefunc_def)
{
  pzlib_filefunc_def->zopen64_file = qiodevice_open64_file_func;
  pzlib_filefunc_def->zread_file = qiodevice_read_file_func;
  pzlib_filefunc_def->zwrite_file = qiodevice_write_file_func;
  pzlib_filefunc_def->ztell64_file = qiodevice_tell64_file_func;
  pzlib_filefunc_def->zseek64_file = qiodevice_seek64_file_func;
  pzlib_filefunc_def->zclose_file = qiodevice_close_file_func;
  pzlib_filefunc_def->zerror_file = qiodevice_error_file_func;
  pzlib_filefunc_def->opaque = NULL;
}