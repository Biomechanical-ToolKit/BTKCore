/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/MotionFile/Formats/C3D/C3DRecord.h"

namespace Open3DMotion
{
	class MapC3DParameters : public MapElement
	{
	public:
		MapC3DParameters();

		virtual ~MapC3DParameters();
		
	public:

		virtual TreeValue* ToTree() const;

		virtual void FromTree(const TreeValue* v);

		void FromFile(const C3DMachine& format, std::istream& is);

		void ToFile(const C3DMachine& format, std::ostream& os) const;

		void AddRecord(C3DRecord* record);

		void RemoveRecord(const char* group, const char* name);

		void MergeFrom(const MapC3DParameters& other);

	public:

		size_t FileBytes() const;

		size_t NumRecords() const
		{ return records.size(); }

		const C3DRecord* GetRecord(const char* groupname, const char* recordname) const;

		template<class RecordType> const RecordType* GetRecordType(const char* groupname, const char* recordname) const
		{
			const C3DRecord* rec = GetRecord(groupname, recordname);
			return (rec != NULL) ? dynamic_cast<const RecordType*> ( rec ) : NULL;
		}

		char GetByte(const char* groupname, const char* recordname, char defaultvalue = 0L, size_t index = 0) const
		{
			const C3DRecordByte* rec = GetRecordType<C3DRecordByte> (groupname, recordname);
			return ((rec != NULL) && (index < rec->Data().size())) ? rec->Data()[index] : defaultvalue;				
		}

		Int16 GetInteger(const char* groupname, const char* recordname, Int16 defaultvalue = 0, size_t index = 0) const
		{
			const C3DRecordInteger* rec = GetRecordType<C3DRecordInteger> (groupname, recordname);
			return ((rec != NULL) && (index < rec->Data().size())) ? rec->Data()[index] : defaultvalue;				
		}

		float GetReal(const char* groupname, const char* recordname, float defaultvalue = 0.0f, size_t index = 0) const
		{
			const C3DRecordReal* rec = GetRecordType<C3DRecordReal> (groupname, recordname);
			return ((rec != NULL) && (index < rec->Data().size())) ? rec->Data()[index] : defaultvalue;				
		}

		bool GetText(std::string& str, const char* groupname, const char* recordname, size_t index = 0UL) const
		{
			const C3DRecordText* rec = GetRecordType<C3DRecordText> (groupname, recordname);
			if (rec != NULL && index < rec->Data().size())
			{
				str = rec->Data()[index];
				return true;
			}
			else
			{
				return false;
			}
		}

	protected:

		const C3DRecord* GetRecord(Int32 group_id, const char* recordname) const;

		bool GetGroupID(Int32& group_id, const char* groupname) const;

	protected:
		std::list<C3DRecord*> records;
	};

}
