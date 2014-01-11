/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_ARRAY_SIMPLE_VALUE_H_
#define _ORMPP_MAP_ARRAY_SIMPLE_VALUE_H_

#include "Open3DMotion/OpenORM/Mappings/MapElement.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Leaves/TreeSimpleValue.h"

namespace Open3DMotion
{
	template<class TreeValueClass, class DataType> class MapArraySimpleValue : public MapElement
	{
	public:
		MapArraySimpleValue(const char* _elementname) :
			elementname(_elementname)
		{
		}

		virtual ~MapArraySimpleValue()
		{
			Clear();
		}

		void Clear()
		{ elementarray.clear(); }

		size_t NumElements() const
		{ return elementarray.size(); }

		const std::string ElementName() const
		{ return elementname; }

		void Add(const DataType& x)
		{ ChildValueChanged(); elementarray.push_back(x); }

		const DataType& operator[] (size_t index) const
		{ return elementarray[index]; }

		DataType& operator[] (size_t index)
		{ ChildValueChanged(); return elementarray[index]; }

		virtual TreeValue* ToTree() const
		{
			TreeList* cpy = new TreeList(elementname.c_str());
			for (typename std::vector<DataType>::const_iterator i( elementarray.begin() ); i != elementarray.end(); i++)
			{
				cpy->Add(new TreeValueClass( *i ));
			}
			return cpy;
		}

		virtual void FromTree(const TreeValue* v)
		{
			Clear();
			const TreeList* src = TreeValueCast<TreeList> (v);
			if (src != NULL)
			{
				if (src->ElementName().compare(elementname) == 0)
				{
					const std::vector<TreeValue*>& src_array = src->ElementArray();
					for (std::vector<TreeValue*>::const_iterator i(src_array.begin()); i != src_array.end(); i++)
					{
						const TreeValueClass* src_element = TreeValueCast<TreeValueClass> (*i);
						if (src_element != NULL)
						{
							elementarray.push_back(src_element->Value());
						}
					}
				}
			}
			ChildValueChanged(); 
		}


	private:
		std::string elementname;
		std::vector<DataType> elementarray;
	};

}
#endif
