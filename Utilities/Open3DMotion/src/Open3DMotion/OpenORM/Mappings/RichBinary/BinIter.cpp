/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"

namespace Open3DMotion
{
	void __bin_iter_compilation_test__()
	{
		RichBinary bin("testcode");
		BinConstIter1<Int32> ic1(bin, "one", 1);
		BinIter1<Int32> i1(bin, "one", 1);
		BinConstIter2<Int32, double> ic2(bin, "one", 1, "two", 1);
		BinIter2<Int32, double> i2(bin, "one", 1, "two", 1);
		BinConstIter3<Int32, double, char> ic3(bin, "one", 1, "two", 1, "three", 1);
		BinIter3<Int32, double, char> i3(bin, "one", 1, "two", 1, "three", 1);
	};

}
