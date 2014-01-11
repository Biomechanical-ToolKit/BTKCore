/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/C3D/C3DRecord.h"
#include <istream>

namespace Open3DMotion
{
	using namespace std;

  //--------------------------------------------------------------
  // static function for creation from file
  // - used when loading c3d's from disk

  C3DRecord* C3DRecord::FromFile(const C3DMachine& machine, istream& is)
  {
    C3DRecord* rec(0);
    char length(0);
    string name, desc;
    char group_id(0);

    // name length - zero for end of records, -ve for locked
    C3DMachine::ReadByte((UInt8&)length, is);
    if (length == 0)
		{
			// null name - means end of records - there will be another terminating byte
			is.seekg(1, ios::cur);

			// indicate no more records
      return NULL;
		}

    // locked if -ve
    bool lock(false);
    if (length < 0)
    {
      length *= -1;
      lock = true;
    }

    // group id: -ve indicates a group
    C3DMachine::ReadByte((UInt8&)group_id, is);

    // read name
    name.resize(length);
		if (length)
		{
			is.read(&name[0], length);
		}

    // skip ptr to next
    is.seekg(2,ios::cur);

    // get the data
    if (group_id < 0)
    {
      // it's a group
      rec = new C3DRecordGroup();
    }
    else
    {
      // it's a parameter

      // read type
      UInt8 t(0);
      C3DMachine::ReadByte(t, is);
      switch (t)
      {
      case 0xFF:
        // text
        rec = new C3DRecordText();
        break;

      case 0x01:
        // byte
        rec = new C3DRecordByte();
        break;

      case 0x02:
        // int
        rec = new C3DRecordInteger();
        break;

      case 0x04:
        // float
        rec = new C3DRecordReal();
        break;

      default:
        throw(MotionFileException(MotionFileException::formaterror, "C3D unrecognised data type in parameters"));
      }
    }

    // set record name and group id
    rec->name = name;
		rec->group_id = group_id;
		rec->locked = lock;

    // read the data
    rec->ValueFromFile(machine, is);

    // read description
    C3DMachine::ReadByte((UInt8&)length, is);
    desc.resize(length);
		if (length)
		{
			is.read(&desc[0], length);
		}
		rec->description = desc;

    // was ok
    return rec;
  }

	void C3DRecord::ToFile(const C3DMachine& machine, std::ostream& os) const
	{
    // name length: -ve indicates lock
    C3DMachine::WriteByte(locked ? 
      (UInt8)(-(char)name.size()) : (char)name.size(), os);

    // group id: -ve sign indicates a group descriptor with no data
		char idchar = (char)(GroupIDSign() * group_id);
    C3DMachine::WriteByte((UInt8)idchar, os);

    // name chars
    os.write(name.c_str(),(streamsize)name.size());

    // ptr to next
    machine.WriteWord((UInt16)(FileBytes()-name.size()-2),os);

		// write value
		ValueToFile(machine, os);

    // description
    C3DMachine::WriteByte((UInt8)description.size(),os);
    os.write(description.c_str(),(streamsize)description.size());
  }

	//--------------------------------------------------------
  // static function for creation based on tree parameters
  // - used when saving an existing c3d file

  C3DRecord* C3DRecord::FromTree(const TreeCompound* rec, const char* name, Int32 group_id)
  {
		C3DRecord* newrec(NULL);

		const TreeString* typestring = rec->GetType<TreeString>("type");
		if (typestring != NULL)
		{
			// make new object
			if (typestring->Value() == "Text")
			{
				newrec = new C3DRecordText();
			}
			else if (typestring->Value() == "Byte")
			{
				newrec = new C3DRecordByte();
			}
			else if (typestring->Value() == "Integer")
			{
				newrec = new C3DRecordInteger();
			}
			else if (typestring->Value() == "Real")
			{
				newrec = new C3DRecordReal();
			}

			if (rec != NULL)
			{
				// set name and group
				newrec->name = name;
				newrec->group_id = group_id;

				// retrieve description
				const TreeString* description = rec->GetType<TreeString> ("description");
				if (description != NULL)
					newrec->description = description->Value();

				// retrieve lock-status
				const TreeBool* locked = rec->GetType<TreeBool> ("locked");
				if (locked != NULL)
					newrec->locked = locked->Value();

				// type-dependent read of value
				newrec->ValueFromTree(rec);
			}
		}

    return newrec;
  }

	void C3DRecord::ToTree(TreeCompound* group) const
	{
		// new record item
		TreeCompound* rec = new TreeCompound;
		
		// set common data
		rec->Set("type", new TreeString(TreeID()));
		rec->Set("description", new TreeString(description.c_str()));
		rec->Set("locked", new TreeBool(locked));
		
		// set type-specific value info
		ValueToTree(rec);

		// append to map using name
		group->Set(name.c_str(), rec);
	}

	void C3DRecordData::ValueToFile(const C3DMachine& machine, std::ostream& os) const
	{
    // type id
    C3DMachine::WriteByte(FileID(),os);

    // dimensions
    C3DMachine::WriteByte((UInt8)range.size(),os);
    for (size_t i = 0; i < range.size(); i++)
      C3DMachine::WriteByte((UInt8)range[i], os);

    // data
    ValueElementsToFile(machine, os);
	}

	void C3DRecordData::ValueFromFile(const C3DMachine& machine, std::istream& is)
	{
		// read dimensions
		UInt8 dimensions(0);
		C3DMachine::ReadByte(dimensions, is);

		// populate range array
    for (size_t i = 0; i < dimensions; i++)
		{
			UInt8 d(0);
			C3DMachine::ReadByte(d, is);
			range.push_back(d);
		}
	
		// read data
   ValueElementsFromFile(machine, is);
	}

	void C3DRecordData::ValueToTree(TreeCompound* rec) const
	{
		list<TreeValue*> valuelist;
		size_t total_elements = TotalElements();
		for (size_t i  = 0; i < total_elements; i++)
		{
			valuelist.push_back( ValueElementToTree(i) );
		}

		for (size_t i = 0; i < range.size(); i++)
		{
			SplitSubLists(valuelist, range[i]);
		}

		TreeValue* value = valuelist.front();

		rec->Set("value", value);
	}

	void C3DRecordData::SplitSubLists(list<TreeValue*>& expandedlist, size_t split)
	{
		std::list<TreeValue*> splitlist;

		TreeList* sublist = NULL;
		size_t index = 0;
		for (list<TreeValue*>::const_iterator iter( expandedlist.begin() ); iter != expandedlist.end(); iter++, index++)
		{
			if (index % split == 0)
			{
				sublist = new TreeList("ArrayItem");
				splitlist.push_back(sublist);
			}

			sublist->Add( *iter );
		}

		// replace with expanded list
		expandedlist = splitlist;
	}

	void C3DRecordData::ValueFromTree(const TreeCompound* rec)
	{
		// get value object
		const TreeValue* value = rec->Get("value");

		// recurse select all child nodes and populate the 'range' and value arrays
		ExpandSubLists(value);
	}

	void C3DRecordData::ExpandSubLists(const TreeValue* value, size_t layer /*=1*/)
	{
		const TreeList* sublist = TreeValueCast<TreeList> ( value );
		if ( sublist == NULL )
		{
			AddValueElementFromTree( value );
		}
		else
		{
			if (range.size() < layer)
			{
				range.insert(range.begin(), sublist->NumElements());
			}

			for (vector<TreeValue*>::const_iterator i( sublist->ElementArray().begin() );  i != sublist->ElementArray().end(); i++)
			{
				ExpandSubLists(*i, layer+1);
			}
		}
	}

	void C3DRecordByte::ValueElementsToFile(const C3DMachine& /*machine*/, std::ostream& os) const
	{
		os.write(&data[0], data.size());
	}

	void C3DRecordByte::ValueElementsFromFile(const C3DMachine& /*machine*/, std::istream& is)
	{
		size_t numbytes = TotalElements();
		data.resize(numbytes, 0);
		is.read(&data[0], numbytes);
	}

	TreeValue* C3DRecordByte::ValueElementToTree(size_t elementindex) const
	{
		return new TreeInt32( data[elementindex] );
	}

	void C3DRecordByte::AddValueElementFromTree(const TreeValue* v)
	{
		const TreeInt32* t = TreeValueCast<TreeInt32> (v);
		char w = (t != NULL) ? (char)t->Value() : 0;
		data.push_back(w);
	}

	void C3DRecordInteger::ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const
	{
		for (vector<Int16>::const_iterator i( data.begin() ); i != data.end(); i++)
			machine.WriteWord(*i, os);
	}

	void C3DRecordInteger::ValueElementsFromFile(const C3DMachine& machine, std::istream& is)
	{
		size_t numwords = TotalElements();
		data.resize(numwords);
		for (vector<Int16>::iterator i( data.begin() ); i != data.end(); i++)
		{
			UInt16 w(0);
			machine.ReadWord(w, is);
			*i = (Int16)w;
		}
	}

	TreeValue* C3DRecordInteger::ValueElementToTree(size_t elementindex) const
	{
		return new TreeInt32( data[elementindex] );
	}

	void C3DRecordInteger::AddValueElementFromTree(const TreeValue* v)
	{
		const TreeInt32* t = TreeValueCast<TreeInt32> (v);
		Int16 w = (t != NULL) ? (Int16)t->Value() : 0;
		data.push_back(w);
	}

	void C3DRecordReal::ValueElementsToFile(const C3DMachine& machine, std::ostream& os) const
	{
		for (vector<float>::const_iterator i( data.begin() ); i != data.end(); i++)
			machine.WriteFloat(*i, os);
	}

	void C3DRecordReal::ValueElementsFromFile(const C3DMachine& machine, std::istream& is)
	{
		size_t numreal = TotalElements();
		data.resize(numreal);
		for (vector<float>::iterator i( data.begin() ); i != data.end(); i++)
		{
			float f(0.0f);
			machine.ReadFloat(f, is);
			*i = f;
		}
	}

	TreeValue* C3DRecordReal::ValueElementToTree(size_t elementindex) const
	{
		return new TreeFloat64( data[elementindex] );
	}

	void C3DRecordReal::AddValueElementFromTree(const TreeValue* v)
	{
		const TreeFloat64* t = TreeValueCast<TreeFloat64> (v);
		float w = (t != NULL) ? (float)t->Value() : 0;
		data.push_back(w);
	}

	void C3DRecordText::ValueToTree(TreeCompound* rec) const
	{
		rec->Set("length", new TreeInt32(range[0]));
		TreeList* textlist = new TreeList("ArrayItem");
		rec->Set("value", textlist);
		for (vector<string>::const_iterator i( data.begin() ); i != data.end(); i++)
		{
			textlist->Add( new TreeString(i->c_str()) );
		}
	}

	void C3DRecordText::ValueFromTree(const TreeCompound* rec)
	{
		const TreeInt32* textlength = rec->GetType<TreeInt32>("length");
		const TreeList* textlist = rec->GetType<TreeList>("value");
		if (textlength != NULL && textlist != NULL)
		{
			for (size_t i = 0; i < textlist->NumElements(); i++)
			{
				const TreeString* s = TreeValueCast<TreeString>( textlist->ElementArray()[i] );
				if (s != NULL)
				{
					data.push_back( s->Value() );
				}
			}

			if (data.size())
			{
				range.push_back(textlength->Value());
				range.push_back(data.size());
			}
		}
	}

	void C3DRecordText::ValueElementsToFile(const C3DMachine& /*machine*/, std::ostream& os) const
	{
		size_t fixedlength = range[0];
		for (vector<string>::const_iterator i( data.begin() ); i != data.end(); i++)
		{
			if (i->size() >= fixedlength)
			{
				os.write(i->c_str(), fixedlength);
			}
			else
			{
				string padded = *i;
				padded.append(fixedlength - i->size(), ' ');
				os.write(padded.c_str(), fixedlength);
			}
		}
	}

	void C3DRecordText::ValueElementsFromFile(const C3DMachine& /*machine*/, std::istream& is)
	{
		size_t fixedlength = range[0];
		size_t numstrings = TotalElements() / fixedlength;
		for (size_t i = 0; i < numstrings; i++)
		{
			string str(fixedlength, ' ');
			is.read(&str[0], fixedlength);
			RemoveTrailingSpace(str);
			data.push_back(str);
		}
	}

	void C3DRecordText::RemoveTrailingSpace(string& str)
	{
		size_t nspace = str.find_last_not_of(' ') + 1;
		if (nspace < str.size())
		{
			str[nspace] = '\0';
			str.resize(nspace);
		}
	}

	TreeValue* C3DRecordText::ValueElementToTree(size_t /*elementindex*/) const
	{
		return NULL;
	}

	void C3DRecordText::AddValueElementFromTree(const TreeValue* /*v*/)
	{
	}
}
