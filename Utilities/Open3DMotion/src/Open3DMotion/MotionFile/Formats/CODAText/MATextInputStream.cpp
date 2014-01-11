/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/CODAText/MATextInputStream.h"
#include <stdio.h>

namespace Open3DMotion
{
	MATextInputStream::MATextInputStream(std::istream& _is) :
		is(_is)
	{
	}

	void MATextInputStream::SkipLine()
	{
		int c = EOF;
		while ((c = is.get()) != EOF && c != '\n')
			;
	}

	void MATextInputStream::ReadTextRow(std::vector<std::string>& text)
	{
		int c = EOF;
		std::string curstring;
		while ((c = is.get()) != EOF)
		{
			if (c == '\t' || c == '\n')
			{
				// remove trailing linefeed
				if (curstring.length() >= 1 && curstring[curstring.length()-1] == '\r')
					curstring = curstring.substr(0, curstring.length()-1);

				// remove trailing space (occasionally get these)
				if (curstring.length() >= 1 && curstring[curstring.length()-1] == ' ')
					curstring = curstring.substr(0, curstring.length()-1);

				// place in array
				text.push_back(curstring);

				// quit at end of line
				if (c == '\n')
					break;

				// clear for next col
				curstring.clear();
			}
			else
			{
				curstring += c;
			}
		}
	}

	bool MATextInputStream::ReadDataRow(std::vector<double>& data, size_t count)
	{
		for (size_t icol = 0; icol < count; icol++)
		{
			char buf[256] = "";
			for (size_t ichar = 0; ichar < 256; ichar++)
			{
				int c = is.get();
				
				if (c == '\n' && icol != (count-1))
					return false;	// some numbers missing

				if (c == '\t' || c == '\n')
					break; // end of this field

				buf[ichar] = (char)c;	// store field
			}

			// scan
			double value;
			if (sscanf(buf, " %lf ", &value) != 1)
				return false;

			// put onto vector
			data.push_back(value);
		}

		return true;
	}
}
