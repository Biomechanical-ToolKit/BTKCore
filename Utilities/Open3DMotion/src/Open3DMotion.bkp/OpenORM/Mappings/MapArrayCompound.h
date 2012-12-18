/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"

namespace Open3DMotion
{
	template<class MapCompoundClass> class MapArrayCompound : public MapElement
	{
	public:
		MapArrayCompound(const char* _elementname, size_t initelements = 0) :
			elementname(_elementname)
		{
			if (initelements)
			{
				for (size_t i = 0; i < initelements; i++)
				{
					Add( new MapCompoundClass() );
				}
			}
		}

		MapArrayCompound(const MapArrayCompound<MapCompoundClass>& src)
		{
			operator=( src );
		}

		virtual ~MapArrayCompound()
		{
			Clear();
		}

	public:

		const MapArrayCompound<MapCompoundClass>& operator=(const MapArrayCompound<MapCompoundClass>& src)
		{
			Clear();
			for (typename std::vector<MapCompoundClass*>::const_iterator i( src.elementarray.begin() ); i != src.elementarray.end(); i++)
			{
				MapCompoundClass* element = new MapCompoundClass;
				*element = *(*i);
				Add( element ); 
			}
			return *this;
		}

		void Clear()
		{
			for (typename std::vector<MapCompoundClass*>::iterator i( elementarray.begin() ); i != elementarray.end(); i++)
			{
				delete *i;
			}
			elementarray.clear();
		}

		virtual TreeValue* ToTree() const
		{
			TreeList* cpy = new TreeList(elementname.c_str());
			for (typename std::vector<MapCompoundClass*>::const_iterator i( elementarray.begin() ); i != elementarray.end(); i++)
			{
				cpy->Add( (*i)->ToTree() );
			}
			return cpy;
		}

		virtual void FromTree(const TreeValue* v)
		{
			Clear();
			const TreeList* listsrc = TreeValueCast<TreeList> (v);
			if (listsrc != NULL)
			{
				if (listsrc->ElementName().compare(elementname) == 0)
				{
					const std::vector<TreeValue*>& src_array = listsrc->ElementArray();
					for (std::vector<TreeValue*>::const_iterator i(src_array.begin()); i != src_array.end(); i++)
					{
						MapCompoundClass* dest_map = new MapCompoundClass();
						dest_map->FromTree(*i);
						Add(dest_map);
					}
				}
			}
			else
			{
				// can consider a compound object as a list of one element
				// provided it has just one member object
				// and that member's name matches our element name
				const TreeCompound* compoundsrc = TreeValueCast<TreeCompound> (v);
				if (compoundsrc != NULL && 
					  (compoundsrc->NumElements() == 1) && 
						(compoundsrc->Node(0)->Name().compare(elementname) == 0))
				{
					MapCompoundClass* dest_map = new MapCompoundClass();
					dest_map->FromTree(compoundsrc->Node(0)->Value());
					Add(dest_map);
				}
			}
		}

		/*
			Sumamry
			Add object pointer and take ownership (so will be deleted during destructor)
		 */
		void Add(MapCompoundClass* element)
		{
			element->SetParent(this);
			elementarray.push_back(element);
		}

		/*
		  Summary
			Take copy of object and add pointer to the copy (not to the original)
		*/
		void Add(const MapCompoundClass& src)
		{
			MapCompoundClass* element = new MapCompoundClass();
			TreeValue* src_value = src.ToTree();
			element->FromTree(src_value);
			delete src_value;
			Add( element );
		}

		size_t NumElements() const
		{ return elementarray.size(); }

		const std::string& ElementName() const
		{ return elementname; }

		const MapCompoundClass& operator[] (size_t index) const
		{ return *elementarray[index]; }

		MapCompoundClass& operator[] (size_t index)
		{ return *elementarray[index]; }

	private:
		std::string elementname;
		std::vector<MapCompoundClass*> elementarray;
	};

}
