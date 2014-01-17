/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/C3D/MapC3DParameters.h"

namespace Open3DMotion
{
	using namespace std;

	MapC3DParameters::MapC3DParameters()
	{
	}

	MapC3DParameters::~MapC3DParameters()
	{
		for (list<C3DRecord*>::iterator i( records.begin() ); i != records.end(); i++)
			delete *i;
		records.clear();
	}

	void MapC3DParameters::FromFile(const C3DMachine& machine, istream& is)
	{
		// read all records
		C3DRecord* rec(NULL);
		while ((rec = C3DRecord::FromFile(machine, is)) != NULL)
    {
			records.push_back(rec);
    }

		// flag as changed
		ChildValueChanged();
	}

	void MapC3DParameters::ToFile(const C3DMachine& machine, std::ostream& os) const
	{
		// write each record
		for (list<C3DRecord*>::const_iterator i( records.begin() ); i != records.end(); i++)
			(*i)->ToFile(machine, os);

		// write param delimiter
    machine.WriteWord(0x0000, os);
	}

	void MapC3DParameters::MergeFrom(const MapC3DParameters& other)
	{
		// find id greater than any existing groups in case we need to add some new ones
		list<C3DRecord*>::const_iterator i;
		Int32 new_group_id = 1;
		for (i = records.begin(); i != records.end(); i++)
		{
			if ( (*i)->AbsGroupID() >= new_group_id )
				new_group_id = (*i)->AbsGroupID() + 1;
		}

		// this maps their group id's onto ours
		map<Int32, Int32> convert_their_id;

		// ensure we have all the same group records, add if not
		for (i = other.records.begin(); i != other.records.end(); i++)
		{
			const C3DRecordGroup* g = dynamic_cast<const C3DRecordGroup*> ( *i );
			if (g != NULL)
			{
				Int32 our_group_id(0L);
				if (!GetGroupID(our_group_id, g->Name().c_str()))
				{
					// make new group if none exists
					our_group_id = new_group_id++;
					AddRecord(new C3DRecordGroup(our_group_id, g->Name().c_str(), g->Description().c_str()));
				}

				// fill in the conversion map
				convert_their_id[g->AbsGroupID()] = our_group_id;
			}
		}

		// now add any records we don't already have
		// ensure we have all the same group records, add if not
		for (i = other.records.begin(); i != other.records.end(); i++)
		{
			const C3DRecordData* d = dynamic_cast<const C3DRecordData*> ( *i );
			if (d != NULL)
			{
				Int32 our_group_id = convert_their_id[d->AbsGroupID()];
				if (GetRecord(our_group_id, d->Name().c_str()) == NULL)
				{
					AddRecord(d->Clone(our_group_id));
				}
			}
		}
	}

	size_t MapC3DParameters::FileBytes() const
	{
		size_t size(0);

		// record size
		for (list<C3DRecord*>::const_iterator i( records.begin() ); i != records.end(); i++)
			size += (*i)->FileBytes();

		// delimiter size
		size += 2;

		return size;
	}

	TreeValue* MapC3DParameters::ToTree() const
	{
		TreeCompound* tree = new TreeCompound;

		// create group descriptions and create map according to ID
		map<Int32, TreeCompound*> groupmap;
		for (list<C3DRecord*>::const_iterator i( records.begin() ); i != records.end(); i++)
		{
			const C3DRecordGroup* g = dynamic_cast<const C3DRecordGroup*>( *i );
			if (g != NULL)
			{
				// new group populated only with description item
				TreeCompound* group = new TreeCompound;
				group->Set("groupdescription", new TreeString(g->Description().c_str()));

				// put group into tree (using name as key)
				tree->Set(g->Name().c_str(), group);

				// make map for local use
				groupmap[g->AbsGroupID()] = group;
			}
		}

		// now add data items
		for (list<C3DRecord*>::const_iterator i( records.begin() ); i != records.end(); i++)
		{
			const C3DRecordData* d = dynamic_cast<const C3DRecordData*>( *i );
			if (d != NULL)
			{
				// look up the group using record index
				map<Int32, TreeCompound*>::iterator igroup = groupmap.find( d->AbsGroupID() );

				// assuming group exists for this id, populate it
				if (igroup != groupmap.end())
				{
					TreeCompound* group = igroup->second;
					d->ToTree(group);
				}
			}
		}

		return tree;
	}

	void MapC3DParameters::FromTree(const TreeValue* v)
	{
		const TreeCompound* tree = TreeValueCast<TreeCompound> (v);
		Int32 current_group_id = 1L;
		if (tree != NULL)
		{
			size_t maxgroups = tree->NumElements();
			for (size_t igroup = 0; igroup < maxgroups; igroup++)
			{
				const TreeCompound* group = TreeValueCast<TreeCompound> (tree->Node(igroup)->Value());
				if (group != NULL)
				{
					// this group item
					const TreeString* treedesc = group->GetType<TreeString> ( "groupdescription" );
					const char* desc = (treedesc != NULL) ? treedesc->Value().c_str() : "";
					C3DRecordGroup* g = new C3DRecordGroup(current_group_id, tree->Node(igroup)->Name().c_str(), desc);
					records.push_back(g);

					// read sub-items
					size_t maxsubitems = group->NumElements();
					for (size_t irec = 0; irec < maxsubitems; irec++)
					{
						const TreeCompound* rec = TreeValueCast<TreeCompound>( group->Node(irec)->Value() );
						if (rec != NULL)
						{
							C3DRecord* d = C3DRecord::FromTree(rec, group->Node(irec)->Name().c_str(), current_group_id);
							records.push_back(d);
						}
					}

					// increment ready for next group
					current_group_id++;
				}
			}
		}
	}

	void MapC3DParameters::RemoveRecord(const char* groupname, const char* recordname)	
	{
		C3DRecord* rec = const_cast<C3DRecord*>( GetRecord(groupname, recordname) );
		if (rec)
		{
			records.remove(rec);
		}
	}

	const C3DRecord* MapC3DParameters::GetRecord(const char* groupname, const char* recordname) const
	{
		Int32 group_id(0);
		if (GetGroupID(group_id, groupname))
		{
			return GetRecord(group_id, recordname);
		}
		return NULL;
	}

	const C3DRecord* MapC3DParameters::GetRecord(Int32 group_id, const char* recordname) const
	{
		for (list<C3DRecord*>::const_iterator irec( records.begin() ); irec != records.end(); irec++)
		{
			if ( ((*irec)->AbsGroupID() == group_id) && ((*irec)->Name().compare(recordname) == 0))
				return *irec;
		}
		return NULL;
	}

	void MapC3DParameters::AddRecord(C3DRecord* record)
	{
		records.push_back(record);
		ChildValueChanged();
	}


	bool MapC3DParameters::GetGroupID(Int32& group_id, const char* groupname) const
	{
		for (list<C3DRecord*>::const_iterator irec( records.begin() ); irec != records.end(); irec++)
		{
			const C3DRecordGroup* g = dynamic_cast<const C3DRecordGroup*> ( *irec );
			if (g != NULL)
			{
				if (g->Name().compare(groupname) == 0)
				{
					// we found this group - use it's ID
					group_id = g->AbsGroupID();
					return true;
				}
			}
		}

		return false;
	}
}
