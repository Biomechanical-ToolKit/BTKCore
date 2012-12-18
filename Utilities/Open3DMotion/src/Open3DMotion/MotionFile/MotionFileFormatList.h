/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MOTION_FILE_FORMAT_LIST_H_
#define _OPEN3DMOTION_MOTION_FILE_FORMAT_LIST_H_

#include <map>
#include <string>

namespace Open3DMotion
{
	class MotionFileFormat;

	/** Simple hashmap of supported formats indexed by format ID */
  class MotionFileFormatList
  {
	public:
		/** Iterator object derived from STL iterator */
		class ConstIterator : public std::map<std::string, MotionFileFormat*>::const_iterator
		{
		public:

			/** Construct using internal STL iterator */
			ConstIterator(const std::map<std::string, MotionFileFormat*>::const_iterator& iter) :
				std::map<std::string, MotionFileFormat*>::const_iterator( iter )
			{
			}

		public:

			/** Helper to retieve pointer to format */
			MotionFileFormat* Format() const
			{ return ((std::map<std::string, MotionFileFormat*>::const_iterator&) (*this))->second; }
		};

	public:

		/** Construct empty list */
		MotionFileFormatList();

		/** Destroy and delete all member format classes */
		~MotionFileFormatList();

		/** Register new format - will be deleted during destructor */
		void Register(MotionFileFormat* format);

		/** Find format by name 
				@return format if found, NULL otherwise 
			*/
		MotionFileFormat* Find(const std::string& formatid) const;

		/** First available format or NULL if empty list */
		MotionFileFormat* DefaultFormat() const
		{ return default_format; }

		/** Start point for iterators */
		ConstIterator Begin() const
		{ return formatmap.begin(); }

		/** End point for iterators */
		ConstIterator End() const
		{ return formatmap.end(); }

	private:
		std::map<std::string, MotionFileFormat*> formatmap;
		MotionFileFormat* default_format;
  };

}

#endif
