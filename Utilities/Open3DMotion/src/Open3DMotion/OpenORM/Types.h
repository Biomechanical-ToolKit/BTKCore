/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_TYPES_H_
#define _OPEN3DMOTION_TYPES_H_

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#ifdef __GNUC__
#include <stdint.h>
#include <stddef.h>
#endif

#ifdef __GNUC__
#include <string.h>
#define _stricmp strcasecmp
#define _finite isfinite
#endif

#ifndef SIZE_MAX
#include <limits>
#define SIZE_MAX std::numeric_limits<size_t>::max()
#endif

namespace Open3DMotion
{
#ifdef __GNUC__
  typedef uint8_t UInt8;
  typedef int8_t Int8;
  typedef uint16_t UInt16;
  typedef int16_t Int16;
  typedef uint32_t UInt32;
  typedef int32_t Int32;
  typedef int64_t Int64;
  typedef uint64_t UInt64;
#else
	typedef char Int8;
	typedef unsigned char UInt8;
	typedef short Int16;
	typedef unsigned short UInt16;
	typedef long Int32;
	typedef unsigned long UInt32;
	typedef __int64 Int64;
	typedef unsigned __int64 UInt64;
#endif
	typedef float Float32;
	typedef double Float64;
}

#endif
