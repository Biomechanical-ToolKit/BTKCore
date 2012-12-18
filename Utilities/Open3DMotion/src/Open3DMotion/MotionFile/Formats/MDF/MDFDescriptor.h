/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MDF_DESCRIPTOR_H_
#define _OPEN3DMOTION_MDF_DESCRIPTOR_H_

#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"

namespace Open3DMotion
{

	class MDFDescriptor
	{
	public:
	  
    virtual ~MDFDescriptor() {};

    virtual UInt16 FormatVersionNumber() = 0;

    virtual UInt32 MarkerElementSize() = 0;

    virtual UInt16 MarkerWriteScaleMicrons() = 0;

		virtual const char* CreationSoftwareVersion() = 0;

    // move marker data from MDF-style arrays
    virtual void TransferMarker(
      TimeSequence& ts, 
      const UInt8* data,
      const UInt8* inview,
      float scale) = 0;

    // write MDF-format marker from time sequence
    virtual void WriteMarker(
      std::ostream& os, 
      const TimeSequence& marker) = 0;

	};

  // MDF implementation for version 2 (16-bit integer marker) files
  class MDFDescriptorVersion2 : public MDFDescriptor
  {
  public:

		// file format version number
    virtual UInt16 FormatVersionNumber()
    { return 0x0102; }

    // 12 bytes (3 int-16's)
    virtual UInt32 MarkerElementSize()
    { return 6; }

    virtual UInt16 MarkerWriteScaleMicrons()
    { return 100; }

    virtual const char* CreationSoftwareVersion()
    { return "File Format 2"; }

    // move marker data from MDF-style arrays
    // - floating point transfer here
    virtual void TransferMarker(
      TimeSequence& ts, 
      const UInt8* data,
      const UInt8* inview,
      float scale);

    // write MDF-format marker from time sequence
    virtual void WriteMarker(
      std::ostream& os, 
      const TimeSequence& marker);
  };

  // MDF implementation for version 3 (floating pt marker) files
  class MDFDescriptorVersion3 : public MDFDescriptor
  {
  public:

    // file format version number
    virtual UInt16 FormatVersionNumber()
    { return 0x0003; }

    // 12 bytes (3 floats)
    virtual UInt32 MarkerElementSize()
    { return 12; }

    virtual UInt16 MarkerWriteScaleMicrons()
    { return 1000; }

    virtual const char* CreationSoftwareVersion()
    { return "File Format 3"; }

		// move marker data from MDF-style arrays
    // - floating point transfer here
    virtual void TransferMarker(
      TimeSequence& ts, 
      const UInt8* data,
      const UInt8* inview,
      float scale);

    // write MDF-format marker from time sequence
    virtual void WriteMarker(
      std::ostream& os, 
      const TimeSequence& marker);
  };

}

#endif
