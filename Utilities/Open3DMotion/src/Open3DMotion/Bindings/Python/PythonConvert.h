/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_PYTHON_CONVERT_H_
#define _OPEN3DMOTION_PYTHON_CONVERT_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include <Python.h>

namespace Open3DMotion
{

	class PythonConvert
	{
	public:
		static const char default_list_element_name[];

		static PyObject* FromTree(const TreeValue* value);

		static TreeValue* ToTree(PyObject* py_value);

	};

}

#endif
