/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include <iostream>

namespace Open3DMotion
{
	class C3DMachine
	{
  public:
    
    virtual ~C3DMachine() {};

    static void ReadByte(UInt8& x, std::istream& is)
    { is.read((char*)&x,1); }

    static void WriteByte(UInt8 x, std::ostream& os)
    { os.write((const char*)&x,1); }

		virtual UInt8 ProcessorID() const = 0;

    virtual void ReadWord(UInt16& x, std::istream& is) const = 0;

    virtual void WriteWord(UInt16 x, std::ostream& os) const = 0;

    virtual void ReadFloat(float& x, std::istream& is) const = 0;
    
		virtual void WriteFloat(float x, std::ostream& os) const = 0;

  };

  // C3D PC-generated integer
  class C3DMachine_PC : public C3DMachine
  {
  public:
    UInt8 ProcessorID() const
		{ return 84; }

		virtual void ReadWord(UInt16& x, std::istream& is) const;

		virtual void WriteWord(UInt16 x, std::ostream& os) const;

    virtual void ReadFloat(float& x, std::istream& is) const;
    
		virtual void WriteFloat(float x, std::ostream& os) const;
  };


  // C3D DEC-generated
  class C3DMachine_DEC : public C3DMachine
  {
  public:

		UInt8 ProcessorID() const
		{ return 85; }

		virtual void ReadWord(UInt16& x, std::istream& is) const;
    
		virtual void WriteWord(UInt16 x, std::ostream& os) const;

    virtual void ReadFloat(float& x, std::istream& is) const;
    
		virtual void WriteFloat(float x, std::ostream& os) const;
  };

  // C3D SGI-generated
  class C3DMachine_SGI : public C3DMachine
  {
  public:

		UInt8 ProcessorID() const
		{ return 86; }

    virtual void ReadWord(UInt16& x, std::istream& is) const;

		virtual void WriteWord(UInt16 x, std::ostream& os) const;

    virtual void ReadFloat(float& x, std::istream& is) const;
    
		virtual void WriteFloat(float x, std::ostream& os) const;
  };

}
