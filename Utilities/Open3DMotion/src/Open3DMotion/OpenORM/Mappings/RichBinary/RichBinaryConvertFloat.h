/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

namespace Open3DMotion
{
	class TreeValue;
	class BinMemFactory;

	/** Convert 32-bit float fields of rich binary object to 64-bit. */
	TreeValue* RichBinaryConvertFloat32To64(const TreeValue* input, const char* structure_name, const BinMemFactory& memfactory);

	/** Convert 64-bit float fields of rich binary object to 32-bit */
	TreeValue* RichBinaryConvertFloat64To32(const TreeValue* input, const char* structure_name, const BinMemFactory& memfactory);
}
