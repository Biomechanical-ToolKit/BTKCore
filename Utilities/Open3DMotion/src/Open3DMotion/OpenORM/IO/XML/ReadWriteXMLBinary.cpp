/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLBinary.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include <pugixml.hpp>
#include <cstring>

extern "C"
{
#include <b64/cencode.h>
#include <b64/cdecode.h>
}

namespace Open3DMotion
{
	void ReadWriteXMLBinary::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{ 
		// binary object
		const TreeBinary* binary = static_cast<const TreeBinary*>( value );

		// binary data
		writer.OS() << "\n";

		if (binary->SizeBytes())
		{
			// init library
			base64_encodestate state;
			base64_init_encodestate(&state);

			// text buffer
			std::string encoded_data(2*binary->SizeBytes(), ' ');

			// convert
			int numchars = base64_encode_block((const char*)binary->Data(), binary->SizeBytes(), &encoded_data[0], &state);
			numchars += base64_encode_blockend(&encoded_data[0] + numchars, &state);

			// shrink to fit
			encoded_data.resize(numchars);

			// write
			writer.OS() << encoded_data;
		}
	}
		
	TreeValue* ReadWriteXMLBinary::ReadValue(XMLReadingMachine& /*reader*/, const pugi::xml_node& element) const
	{
		// find text element with base64 encoding in it
		const char* text = "";
		for (pugi::xml_node child = element.first_child(); child; child = child.next_sibling())
		{
			if (child.type() == pugi::node_pcdata)
			{
				text = child.value();
				break;
			}
		}

		// evaluate length
		int textlength = strlen(text);

		// oversize buffer for decoded result
		std::vector<UInt8> decoded_binary(textlength);

		// decode
		base64_decodestate state;
		base64_init_decodestate(&state);
		int decoded_length = base64_decode_block(text, textlength, (char*) &decoded_binary[0], &state);
		
		// copy to memory object
		MemoryHandlerBasic binary_handler((size_t)decoded_length);
		memcpy(binary_handler.Data(), &decoded_binary[0], (size_t)decoded_length);
		
		// done
		return new TreeBinary(&binary_handler);
	}
}
