/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/MotionFile/Formats/C3D/C3DMachine.h"

namespace Open3DMotion
{
	using namespace std;

	//----------------------------------------------------------
  // PC  format
  
  void C3DMachine_PC::ReadWord(UInt16& x, istream& is) const
  {
    is.read((char*)&x, 2);
  }

  void C3DMachine_PC::WriteWord(UInt16 x, ostream& os) const
  {
    os.write((const char*)&x, 2);
  }

  void C3DMachine_PC::ReadFloat(float& x, istream& is) const
  {
    is.read((char*)&x, 4);
  }
  
  void C3DMachine_PC::WriteFloat(float x, ostream& os) const
  {
    os.write((const char*)&x, 4);
  }


  //----------------------------------------------------------
  // DEC format
  
  void C3DMachine_DEC::ReadWord(UInt16& x, istream& is) const
  {
    is.read((char*)&x, 2);
  }

  void C3DMachine_DEC::WriteWord(UInt16 x, ostream& os) const
  {
    os.write((const char*)&x, 2);
  }

  void C3DMachine_DEC::ReadFloat(float& x, istream& is) const
  {
    char* b = (char*)&x;
    is.read(b+2, 2);
    is.read(b, 2);
    x *= 0.25;
  }
  
  void C3DMachine_DEC::WriteFloat(float x, ostream& os) const
  {
    x *= 4.0;
    const char* b = (const char*)&x;
    os.write(b+2, 2);
    os.write(b, 2);
  }

  //----------------------------------------------------------
  // SGI format
  
  void C3DMachine_SGI::ReadWord(UInt16& x, istream& is) const
  {
    char* b = (char*)&x;
    is.read(b+1, 1);
    is.read(b, 1);
  }

  void C3DMachine_SGI::WriteWord(UInt16 x, ostream& os) const
  {
    const char* b = (const char*)&x;
    os.write(b+1, 1);
    os.write(b, 1);
  }

  void C3DMachine_SGI::ReadFloat(float& x, istream& is) const
  {
    char* b = (char*)&x;
    is.read(b+3, 1);
    is.read(b+2, 1);
    is.read(b+1, 1);
    is.read(b  , 1);
  }
  
  void C3DMachine_SGI::WriteFloat(float x, ostream& os) const
  {
    const char* b = (const char*)&x;
    os.write(b+3, 1);
    os.write(b+2, 1);
    os.write(b+1, 1);
    os.write(b  , 1);
  }
}
