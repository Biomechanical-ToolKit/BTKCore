/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "PythonConvert.h"
#include "MemoryHandlerPython.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"

namespace Open3DMotion
{
	const char PythonConvert::default_list_element_name[] = "ArrayItem";

	PyObject* PythonConvert::FromTree(const TreeValue* value)
	{
		if (value == NULL)
		{
			Py_RETURN_NONE;
		}
		else if (value->ClassNameMatches(TreeString::classname))
		{
			const TreeString* value_string = static_cast<const TreeString*> (value);
			std::string strcopy( value_string->Value() );
			PyObject* py_string = PyString_FromString(&strcopy[0]);
			return py_string;
		}
		else if (value->ClassNameMatches(TreeBool::classname))
		{
			const TreeBool* value_bool = static_cast<const TreeBool*> (value);
			if (value_bool->Value())
			{
				Py_RETURN_TRUE;
			}
			else
			{
				Py_RETURN_FALSE;
			}
		}
		else if (value->ClassNameMatches(TreeInt32::classname))
		{
			const TreeInt32* value_int32 = static_cast<const TreeInt32*> (value);
			PyObject* py_value = PyLong_FromLong(value_int32->Value());
			return py_value;
		}
		else if (value->ClassNameMatches(TreeFloat64::classname))
		{
			const TreeFloat64* value_float64 = static_cast<const TreeFloat64*> (value);
			PyObject* py_value = PyFloat_FromDouble(value_float64->Value());
			return py_value;
		}
		else if (value->ClassNameMatches(TreeBinary::classname))
		{
			const TreeBinary* bin = static_cast<const TreeBinary*> (value);
			const MemoryHandlerPython* mem = NamedClassCast<MemoryHandler, MemoryHandlerPython> ( bin->BinMemory() );
			if (mem)
			{
				PyObject* py_bytearray = mem->py_bytearray;
				Py_INCREF(py_bytearray);
				return py_bytearray;
			}
			else
			{
				// Only support binary memory allocated using the Python allocator
				Py_RETURN_NONE;
			}
		}
		else if (value->ClassNameMatches(TreeList::classname))
		{
			const TreeList* value_list = static_cast<const TreeList*> (value);
			PyObject* py_list = PyList_New(1);
			PyList_SetItem(py_list, 0, PyString_FromString(value_list->ElementName().c_str()));
			const std::vector<TreeValue*>& element_array = value_list->ElementArray();
			for (std::vector<TreeValue*>::const_iterator iter(element_array.begin()); iter != element_array.end(); iter++)
			{
				// Use Append here not SetItem 
				// To provide robustness to items we can't process (will just skip them)
				PyObject* py_new_item = PythonConvert::FromTree(*iter);
				if (py_new_item != Py_None)
				{
					PyList_Append(py_list, py_new_item);
				}
				Py_DECREF(py_new_item);
			}
			return py_list;
		}
		else if (value->ClassNameMatches(TreeCompound::classname))
		{
			const TreeCompound* c = static_cast<const TreeCompound*> (value);
			PyObject* py_c = PyDict_New();
			size_t n = c->NumElements();
			for (size_t i = 0; i < n; i++)
			{
				const TreeCompoundNode* node = c->Node(i);
				PyObject* py_new_item = PythonConvert::FromTree(node->Value());
				if (py_new_item != Py_None)
				{
					PyDict_SetItemString(py_c, node->Name().c_str(), py_new_item);
				}
				Py_DECREF(py_new_item);
			}
			return py_c;
		}
		else
		{
			Py_RETURN_NONE;
		}
	}

	TreeValue* PythonConvert::ToTree(PyObject* py_value)
	{
		if (py_value == NULL || py_value == Py_None)
		{
			return NULL;
		}
		else if (PyString_Check(py_value))
		{
			return new TreeString( PyString_AS_STRING(py_value) );
		}
		else if (PyBool_Check(py_value))
		{
			return new TreeBool( (py_value == Py_True) ? true : false );
		}
		else if (PyLong_Check(py_value))
		{
			return new TreeInt32(PyLong_AsLong(py_value));
		}
		else if (PyFloat_Check(py_value))
		{
			return new TreeFloat64(PyFloat_AsDouble(py_value));
		}
		else if (PyByteArray_Check(py_value))
		{
			MemoryHandlerPython mem( py_value );
			return new TreeBinary( &mem );
		}
		else if (PyList_Check(py_value))
		{
			// default element name if none found at start of list
			const char* element_name = default_list_element_name;
			
			// if first element is a string, use it as element name
			Py_ssize_t s = PyList_Size(py_value);
			Py_ssize_t index(0);
			if (s >= 1)
			{
				PyObject* py_obj0 = PyList_GetItem(py_value, 0);
				if (PyString_Check(py_obj0))
				{
					element_name = PyString_AsString(py_obj0);
					++index;
				}
			}

			// copy the rest
			TreeList* result_list = new TreeList(element_name);
			for (; index < s; index++)
			{
				PyObject* py_obj = PyList_GetItem(py_value, index);
				TreeValue* tree_item = PythonConvert::ToTree(py_obj);
				if (tree_item)
				{
					result_list->Add(tree_item);
				}
			}

			return result_list;
		}
		else if (PyDict_Check(py_value))
		{
			TreeCompound* c = new TreeCompound;
			PyObject* key = NULL;
			PyObject* py_element = NULL;
			Py_ssize_t pos = 0;
			while (PyDict_Next(py_value, &pos, &key, &py_element)) 
			{
				if (PyString_Check(key))
				{
					TreeValue* element = PythonConvert::ToTree(py_element);
					if (element != NULL)
					{
						const char* key_string = PyString_AsString(key);
						c->Set(key_string, element);
					}
				}
			}
			return c;
		}
		else
		{
			return NULL;
		}
	}
}
