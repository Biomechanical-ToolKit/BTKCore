/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_ELEMENT_H_
#define _ORMPP_MAP_ELEMENT_H_

#include "Open3DMotion/OpenORM/TreeValue.h"

namespace Open3DMotion
{
	class MapElement
	{
		friend class MapCompound;
		template<class MapCompoundClass> friend class MapArrayCompound;

	public:
		MapElement();

		/** Copy constructor which does nothing as parent element should not
		    be copied when using = to copy from one compound structure to another */
		MapElement(const MapElement& /*src*/) 
			: parent(NULL)
		{  }

		/** Assignment operator which does nothing as parent element should not
		    be copied when using = to copy from one compound structure to another */
		const MapElement& operator=(const MapElement& /*src*/)
		{ return *this; }

	public:
		
		virtual ~MapElement();

		virtual TreeValue* ToTree() const = 0;

		virtual void FromTree(const TreeValue* v) = 0;

		virtual void ChildValueChanged();

	protected:
		void SetParent(MapElement* _parent);

	private:
		MapElement* parent;
	};

}
#endif