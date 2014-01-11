/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/MDF/MDFDescriptor.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include <ostream>

namespace Open3DMotion
{
  void MDFDescriptorVersion2::WriteMarker(
    std::ostream& os, 
    const TimeSequence& marker)
  {
    // number of frames
		UInt16 wElements = (UInt16)marker.NumFrames();
    os.write((const char*)&wElements,2);

    // the 3-D data
		for (TSOccVector3ConstIter iter_ts(marker); iter_ts.HasFrame(); iter_ts.Next())
    {
      double d;
      Int16 s;

      // x
			d = 10.0 * iter_ts.Value()[0];
      s = (d >= 0.0) ? (Int16)(d+0.5) : (Int16)(d-0.5);
      os.write((const char*)&s,2);

      // y
      d = 10.0 * iter_ts.Value()[1];
      s = (d >= 0.0) ? (Int16)(d+0.5) : (Int16)(d-0.5);
      os.write((const char*)&s,2);

      // z
      d = 10.0 * iter_ts.Value()[2];
      s = (d >= 0.0) ? (Int16)(d+0.5) : (Int16)(d-0.5);
      os.write((const char*)&s,2);
    }
  }

  // transfer marker as scaled 16-bit integers
  void MDFDescriptorVersion2::TransferMarker(TimeSequence& ts, 
                                     const UInt8* data,
                                     const UInt8* inview,
                                     float scale)
  {
    const Int16* pos = reinterpret_cast<const Int16*>(data);
		// Int32 num_frames = ts.NumFrames();
    UInt16 wInView(0);
    *(UInt8*)&wInView = *(inview+1);
    *((UInt8*)&wInView+1) = *inview;
    for (TSOccVector3Iter iter_ts(ts); iter_ts.HasFrame(); iter_ts.Next(), pos += 3)
    {
      iter_ts.Value()[0] = pos[0] * scale;
      iter_ts.Value()[1] = pos[1] * scale;
      iter_ts.Value()[2] = pos[2] * scale;

      // extract in-view bit
			UInt16 bitselect = (UInt16)iter_ts.FrameIndex() & 0xF;
      iter_ts.Occluded() = (wInView & (0x8000 >> bitselect)) ? 0 : 1;
      
      // update inview word (every 16 bits)
      if (bitselect == 0xF)
      {
        inview += 2;
        *(UInt8*)&wInView = *(inview+1);
        *((UInt8*)&wInView+1) = *inview;
      }
    }
  }

	void MDFDescriptorVersion3::WriteMarker(
    std::ostream& os, 
    const TimeSequence& marker)
  {
    // number of frames
		UInt16 wElements = (UInt16)marker.NumFrames();
    os.write((const char*)&wElements,2);

    // the 3-D data
		for (TSOccVector3ConstIter iter_ts(marker); iter_ts.HasFrame(); iter_ts.Next())
    {
      float f;
      f = (float)iter_ts.Value()[0];
      os.write((const char*)&f,4);
      f = (float)iter_ts.Value()[1];
      os.write((const char*)&f,4);
      f = (float)iter_ts.Value()[2];
      os.write((const char*)&f,4);
    }
  }

  // transfer marker as floating point
  void MDFDescriptorVersion3::TransferMarker(TimeSequence& ts, 
                                     const UInt8* data,
                                     const UInt8* inview,
                                     float /*scale*/)
  {
    const float* pos = reinterpret_cast<const float*>(data);
    UInt16 wInView;
    *(UInt8*)&wInView = *(inview+1);
    *((UInt8*)&wInView+1) = *inview;
    for (TSOccVector3Iter iter_ts(ts); iter_ts.HasFrame(); iter_ts.Next(), pos += 3)
    {
      // copy 3d
      iter_ts.Value()[0] = (double)pos[0];
      iter_ts.Value()[1] = (double)pos[1];
      iter_ts.Value()[2] = (double)pos[2];

      // extract in-view bit
			UInt16 bitselect = (UInt16)iter_ts.FrameIndex() & 0xF;
      iter_ts.Occluded() = (wInView & (0x8000 >> bitselect)) ? 0 : 1;
      
      // update inview word (every 16 bits)
      if (bitselect == 0xF)
      {
        inview += 2;
        *(UInt8*)&wInView = *(inview+1);
        *((UInt8*)&wInView+1) = *inview;
      }
    }
  }

}
