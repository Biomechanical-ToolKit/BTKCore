/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Mappings/MapCompoundElement.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"
#include "Open3DMotion/OpenORM/Mappings/MapArraySimpleValue.h"
#include "Open3DMotion/MotionFile/Formats/C3D/C3DMachine.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"

#include <vector>
#include <list>

namespace Open3DMotion
{
  // C3D parameter record
  class C3DRecord
  {
  protected:
		C3DRecord() :
			locked(false),
      group_id(0)
    {
    }
	
		C3DRecord(Int32 _group_id, const char* _name, const char* _description) :
			name(_name),
			description(_description),
			locked(false),
			group_id(_group_id)
    {
    }

		C3DRecord(const C3DRecord& src)
		{
			name = src.Name();
			description = src.Description();
			locked = src.Locked();
			group_id = src.AbsGroupID();
		}

	public:
    virtual ~C3DRecord()
    {
    }

  public:

    // type id in files
    virtual UInt8 FileID() const = 0;

    // type id in parameter tree
    virtual const char* TreeID() const = 0;

		// size in file
		virtual size_t FileBytes() const = 0;

		// sign to write for group id (negative group descriptor record)
		virtual Int32 GroupIDSign() const = 0;

	public:

    // read unknown type from parameter tree
    // - must manually delete afterwards
    static C3DRecord* FromTree(const TreeCompound* rec, const char* name, Int32 group_id);

    // read unknown type from file & construct appropriate type
    // - must manually delete afterwards
    static C3DRecord* FromFile(const C3DMachine& machine, std::istream& is);

		/** Write to binary file */
		virtual void ToFile(const C3DMachine& machine, std::ostream& os) const;

		/** Write to tree branch (alongside other records, using name as index) */
		virtual void ToTree(TreeCompound* group) const;

	public:

		const std::string& Name() const
		{ return name; }

		const std::string& Description() const
		{ return description; }

		bool Locked() const
		{ return locked; }

		bool& Locked()
		{ return locked; }

		Int32 AbsGroupID() const
		{ return (group_id >= 0) ? group_id : (-group_id); }

	protected:

		/** Read value-related information from file */
		virtual void ValueFromFile(const C3DMachine& machine, std::istream& is) = 0;

		/** Write value-related information to file */
		virtual void ValueToFile(const C3DMachine& machine, std::ostream& os) const = 0;

		/** Append any value-related information to the record */
		virtual void ValueToTree(TreeCompound* rec) const = 0;

		/** Append any value-related information to the record */
		virtual void ValueFromTree(const TreeCompound* rec) = 0;

	protected:
    
		// data common to all records
		std::string name;
    std::string description;
    bool locked;
    Int32 group_id;
  };

  class C3DRecordGroup : public C3DRecord
  {
	public:
    C3DRecordGroup()
    {
    }

    C3DRecordGroup(Int32 _group_id, const char* _name, const char* _description) :
			C3DRecord(_group_id, _name, _description)
    {
    }
		
	public:
    // type id in files
    virtual UInt8 FileID() const
    { return 0; }

    // type id in parameter tree
    virtual const char* TreeID() const
    { return "Group"; }

    // size occupied in file
		virtual size_t FileBytes() const
		{ return size_t(5) + name.size() + description.size(); }

		// sign to write for group id (negative group descriptor record)
		virtual Int32 GroupIDSign() const
		{ return -1; }

	protected:

		virtual void ValueFromFile(const C3DMachine& machine, std::istream& is) 
		{
		}

		virtual void ValueToFile(const C3DMachine& machine, std::ostream& os) const 
		{
		}

		virtual void ValueToTree(TreeCompound* rec) const
		{
		}

		virtual void ValueFromTree(const TreeCompound* rec)
		{
		}
	};

	class C3DRecordData : public C3DRecord
	{
	protected:
		C3DRecordData()
		{
		}
	
		C3DRecordData(const C3DRecordData& src, Int32 new_group_id) :
			C3DRecord(src)
		{
			group_id = new_group_id;
			range = src.Range();
		}

		C3DRecordData(Int32 _group_id, const char* _name, const char* _description) :
				C3DRecord(_group_id, _name, _description)
		{
		}

	public:
    // size occupied in file
		virtual size_t FileBytes() const
		{ return size_t(7) + name.size() + range.size() + (TotalElements()*ElementSize()) + description.size(); }

		// sign to write for group id (negative group descriptor record)
		virtual Int32 GroupIDSign() const
		{ return 1; }

			// size of each element
    virtual UInt32 ElementSize() const = 0;

    const std::vector<UInt32>& Range() const
    { return range; }

    UInt32 TotalElements() const
    { 
			UInt32 total = 1;
			for (std::vector<UInt32>::const_iterator irange(range.begin()); irange != range.end(); irange++)
				total *= *irange;
			return total;
		}

		virtual C3DRecordData* Clone(Int32 new_group_id) const = 0;

	protected:

		virtual void ValueToFile(const C3DMachine& machine, std::ostream& os) const;

		virtual void ValueFromFile(const C3DMachine& machine, std::istream& is);

		virtual void ValueToTree(TreeCompound* rec) const;

		virtual void ValueFromTree(const TreeCompound* rec);

		virtual void ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const = 0;

		virtual void ValueElementsFromFile(const C3DMachine& machine, std::istream& is) = 0;

		virtual TreeValue* ValueElementToTree(size_t elementindex) const = 0;

		virtual void AddValueElementFromTree(const TreeValue* v) = 0;

	protected:

		void ExpandSubLists(const TreeValue* sublist, size_t layer = 1);

		static void SplitSubLists(std::list<TreeValue*>& sublist, size_t split);

	protected:
		std::vector<UInt32> range;
	};

  class C3DRecordByte : public C3DRecordData
  {
  public:
    C3DRecordByte()
    {
    }

		C3DRecordByte(Int32 _group_id, const char* _name, const char* _description, char value) :
				C3DRecordData(_group_id, _name, _description)
		{
			data.push_back(value);
		}

	protected:
    C3DRecordByte(const C3DRecordByte& src, Int32 new_group_id) :
			C3DRecordData(src, new_group_id),
			data(src.Data())
    {
    }

	public:
    // size of elements
    virtual UInt32 ElementSize() const
    { return 1; }

    // type id in files
    virtual UInt8 FileID() const
    { return 0x01; }

    // type id in parameter tree
    virtual const char* TreeID() const
    { return "Byte"; }

		// data values
		const std::vector<char>& Data() const
		{ return data; }

		virtual C3DRecordByte* Clone(Int32 new_group_id) const 
		{ return new C3DRecordByte(*this, new_group_id); }

	protected:

		virtual void ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const;

		virtual void ValueElementsFromFile(const C3DMachine& machine, std::istream& is);

		virtual TreeValue* ValueElementToTree(size_t elementindex) const;

		virtual void AddValueElementFromTree(const TreeValue* v);

	protected:
		std::vector<char> data;
  };

  class C3DRecordInteger : public C3DRecordData
  {
  public:    
    C3DRecordInteger()
    {
    }

		C3DRecordInteger(Int32 _group_id, const char* _name, const char* _description, Int16 value) :
				C3DRecordData(_group_id, _name, _description)
		{
			data.push_back(value);
		}

		C3DRecordInteger(Int32 _group_id, const char* _name, const char* _description, const std::vector<Int16>& _data) :
				C3DRecordData(_group_id, _name, _description)
		{
			range.push_back(_data.size());
			data = _data;
		}

		C3DRecordInteger(Int32 _group_id, const char* _name, const char* _description, const std::vector<Int16>& _data, const std::vector<UInt32>& _range) :
				C3DRecordData(_group_id, _name, _description)
		{
			range = _range;
			data = _data;
		}

	protected:
    C3DRecordInteger(const C3DRecordInteger& src, Int32 new_group_id) :
			C3DRecordData(src, new_group_id),
			data(src.Data())
    {
    }

	public:
		// size of elements
    virtual UInt32 ElementSize() const
    { return 2; }

    // type id in files
    virtual UInt8 FileID() const
    { return 0x02; }

    // type id in parameter tree
    virtual const char* TreeID() const
    { return "Integer"; }

		// data values
		const std::vector<Int16>& Data() const
		{ return data; }

	protected:

		virtual void ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const;

		virtual void ValueElementsFromFile(const C3DMachine& machine, std::istream& is);

		virtual TreeValue* ValueElementToTree(size_t elementindex) const;

		virtual void AddValueElementFromTree(const TreeValue* v);

		virtual C3DRecordInteger* Clone(Int32 new_group_id) const 
		{ return new C3DRecordInteger(*this, new_group_id); }

	protected:

		std::vector<Int16> data;
	};

  class C3DRecordReal : public C3DRecordData
  {
  public:
    C3DRecordReal()
    {
    }

		C3DRecordReal(Int32 _group_id, const char* _name, const char* _description, float value) :
				C3DRecordData(_group_id, _name, _description)
		{
			data.push_back(value);
		}

		C3DRecordReal(Int32 _group_id, const char* _name, const char* _description, const std::vector<float>& _data) :
				C3DRecordData(_group_id, _name, _description)
		{
			range.push_back(_data.size());
			data = _data;
		}

		C3DRecordReal(Int32 _group_id, const char* _name, const char* _description, const std::vector<float>& _data, const std::vector<UInt32>& _range) :
				C3DRecordData(_group_id, _name, _description)
		{
			range = _range;
			data = _data;
		}

	protected:
    C3DRecordReal(const C3DRecordReal& src, Int32 new_group_id) :
			C3DRecordData(src, new_group_id),
			data(src.Data())
    {
    }

	public:

		// size of elements
    virtual UInt32 ElementSize() const
    { return 4; }

    // type id in files
    virtual UInt8 FileID() const
    { return 0x04; }

    // type id in parameter tree
    virtual const char* TreeID() const
    { return "Real"; }

		// data values
		const std::vector<float>& Data() const
		{ return data; }

	protected:

		virtual void ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const;

		virtual void ValueElementsFromFile(const C3DMachine& machine, std::istream& is);

		virtual TreeValue* ValueElementToTree(size_t elementindex) const;

		virtual void AddValueElementFromTree(const TreeValue* v);

		virtual C3DRecordReal* Clone(Int32 new_group_id) const 
		{ return new C3DRecordReal(*this, new_group_id); }

	protected:

		std::vector<float> data;
 };

  class C3DRecordText : public C3DRecordData
  {
  public:
    C3DRecordText()
    {
    }

		C3DRecordText(Int32 _group_id, const char* _name, const char* _description, const char* value) :
				C3DRecordData(_group_id, _name, _description)
		{
			range.push_back(strlen(value));
			range.push_back(1);
			data.push_back(value);
		}

		C3DRecordText(Int32 _group_id, const char* _name, const char* _description, const std::vector<std::string> _data, size_t maxlen = 32) :
				C3DRecordData(_group_id, _name, _description)
		{
			range.push_back(maxlen);
			range.push_back(_data.size());
			data = _data;
		}

	protected:
    C3DRecordText(const C3DRecordText& src, Int32 new_group_id) :
			C3DRecordData(src, new_group_id),
			data(src.Data())
    {
    }

	public:
			// size of elements
    virtual UInt32 ElementSize() const
    { return 1; }

    // type id in files
    virtual UInt8 FileID() const
    { return 0xFF; }

    // type id in parameter tree
    virtual const char* TreeID() const
    { return "Text"; }

		// data values
		const std::vector<std::string>& Data() const
		{ return data; }

		virtual C3DRecordText* Clone(Int32 new_group_id) const 
		{ return new C3DRecordText(*this, new_group_id); }

	protected:

		virtual void ValueToTree(TreeCompound* rec) const;

		virtual void ValueFromTree(const TreeCompound* rec);

		virtual void ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const;

		virtual void ValueElementsFromFile(const C3DMachine& machine, std::istream& is);

		virtual TreeValue* ValueElementToTree(size_t elementindex) const;

		virtual void AddValueElementFromTree(const TreeValue* v);

		static void RemoveTrailingSpace(std::string& str);

	protected:

		std::vector<std::string> data;
 };

}
