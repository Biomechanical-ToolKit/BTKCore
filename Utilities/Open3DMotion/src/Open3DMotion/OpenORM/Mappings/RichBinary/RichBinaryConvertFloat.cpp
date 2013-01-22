/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactory.h"
#include "RichBinaryConvertFloat.h"

namespace Open3DMotion
{
	template<typename OutputType, typename InputType> TreeValue* RichBinaryConvertFields(const TreeValue* input, const char* structure_name, BinMemFactory& memfactory);

	template<typename OutputType, typename InputType> TreeValue* RichBinaryConvertFields(const TreeValue* input, const char* structure_name, BinMemFactory& memfactory)
	{
		// read input
		RichBinary rb_input(structure_name);
		rb_input.FromTree( input );
		if (rb_input.DataStructure().TotalBytes() == 0)
			return NULL;

		// find fields to convert
		std::vector<BinaryFieldSpec> layout_output;
		const MapArrayCompound<BinaryFieldSpec>& layout_input = rb_input.DataStructure().GetLayout();
		for (size_t ifield = 0; ifield < layout_input.NumElements(); ifield++)
		{
			const BinaryFieldSpec& field = layout_input[ifield];
			if (field.Type.Value().compare(BinaryFieldTypeString<InputType>()) == 0)
			{
				layout_output.push_back(BinaryFieldSpec::FromType<OutputType>(field.Name.Value().c_str(), field.Dimension.Value()));
			}
			else
			{
				layout_output.push_back(field);
			}
		}

		// allocate new binary with new structure
		RichBinary rb_output(structure_name);
		size_t num_elements = rb_input.DataSizeBytes() / rb_input.DataStructure().TotalBytes();
		rb_output.Allocate(layout_output, num_elements, memfactory);

		// copy data
		size_t input_bytes_accum(0);
		size_t output_bytes_accum(0);
		const UInt8* data_input = rb_input.Data();
		UInt8* data_output = rb_output.Data();
		for (size_t ifield = 0; ifield < layout_input.NumElements(); ifield++)
		{
			const BinaryFieldSpec& field_input = layout_input[ifield];
			const BinaryFieldSpec& field_output = layout_output[ifield];
			const UInt8* ptr_input = data_input + input_bytes_accum;
			UInt8* ptr_output = data_output + output_bytes_accum;
			size_t field_size_input(field_input.Bytes);
			size_t field_size_output(field_output.Bytes);
			size_t non_field_size_input = rb_input.DataStructure().TotalBytes() - field_size_input;
			size_t non_field_size_output = rb_output.DataStructure().TotalBytes() - field_size_output;
			if (field_input.Type.Value().compare(BinaryFieldTypeString<InputType>()) == 0)
			{
				// conversion
				size_t ndim = field_input.Dimension;
				for(size_t ielement = 0; ielement < num_elements; ielement++)
				{
					for (size_t idim = 0; idim < ndim; idim++)
					{
						*(OutputType*)(ptr_output) = *(const InputType*)(ptr_input);
						ptr_input += sizeof(InputType);
						ptr_output += sizeof(OutputType);
					}
					ptr_input += non_field_size_input;
					ptr_output += non_field_size_output;
				}
			}
			else
			{
				// straight copy other fields
				for(size_t ielement = 0; ielement < num_elements; ielement++)
				{
					for (size_t ibyte = 0; ibyte < field_size_input; ibyte++)
						*ptr_output++ = *ptr_input++;
					ptr_input += non_field_size_input;
					ptr_output += non_field_size_output;
				}
			}
			input_bytes_accum += field_size_input;
			output_bytes_accum += field_size_output;
		}

		// output
		TreeCompound* compound_output = TreeValueCast<TreeCompound> ( rb_output.ToTree() );

		// copy any additional items to output
		const TreeCompound* compound_input = TreeValueCast<const TreeCompound> ( input );
		size_t num_nodes = compound_input->NumElements();
		for (size_t inode = 0; inode < num_nodes; inode++)
		{
			const TreeCompoundNode* node = compound_input->Node(inode);
			if (compound_output->Get(node->Name().c_str() ) == NULL)
			{
				TreeValue* output_value = node->Value()->NewBlank();
				output_value->CopyFrom( node->Value() );
				compound_output->Set(node->Name().c_str(), output_value);
			}
		}

		return compound_output;
	}

	TreeValue* RichBinaryConvertFloat32To64(const TreeValue* input, const char* structure_name, BinMemFactory& memfactory)
	{
		return RichBinaryConvertFields<double, float> (input, structure_name, memfactory);
	}

	TreeValue* RichBinaryConvertFloat64To32(const TreeValue* input, const char* structure_name, BinMemFactory& memfactory)
	{
		return RichBinaryConvertFields<float, double> (input, structure_name, memfactory);
	}
}
