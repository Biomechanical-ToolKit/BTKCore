/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "MotionFileFormat.h"
#include <string.h>

namespace Open3DMotion
{
	using namespace std;

  // construct new (abstract) file format
  MotionFileFormat::MotionFileFormat(
    const char* _formatid,
    const char* _summary, 
    const char* _extensions) :
    strFormatID(_formatid),
    strSummary(_summary),
    strExtensions(_extensions)
  {		
		// parse comma-separated list
		const char* ptr = strExtensions;
		string ext;
		while (*ptr != L'\0')
		{
			if (*ptr == L',')
			{
				extension_array.push_back(ext);
				ext.clear();
			}
			else
			{
				ext += *ptr;
			}
			ptr++;
		}
		extension_array.push_back(ext);
  }


	bool MotionFileFormat::ProbeTextString(istream& is, const char* searchstring, size_t searchrange)
	{
		// buffer of same length as search string
    size_t stringlength = strlen(searchstring);
    char* testbuffer = new char[stringlength];
		
		// read as much as we can into buffer (usually will fill it)
		memset(testbuffer, 0, stringlength);
		is.read(testbuffer, stringlength);
    
		bool found(false);
    for (size_t itest = 0; itest < searchrange; itest++)
    {
			// compare current buffer with search
			if (strncmp(searchstring, testbuffer, stringlength) == 0)
      {
        found = true;
        break;
      }

			// shift buffer along
      for (size_t ich = 1; ich < stringlength; ich++)
        testbuffer[ich-1] = testbuffer[ich];

			// read next character
      is.read(testbuffer+stringlength-1, 1);
    }

		// done with buffer 
		delete[] testbuffer;
    
		// return result
    return found;	
	}
}

