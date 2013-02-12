/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatXMove.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatDescriptor.h"
#include <set>
#include "Open3DMotion/OpenORM/IO/XML/XMLReadingMachine.h"
#include "Open3DMotion/OpenORM/IO/XML/XMLWritingMachine.h"
#include "Open3DMotion/MotionFile/Formats/XMove/XMLReadingMachineLegacy.h"
#include "Open3DMotion/MotionFile/Formats/XMove/XMLWritingMachineLegacy.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinaryConvertFloat.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <pugixml.hpp>
#include <memory>

namespace Open3DMotion
{	
	FileFormatXMove::FileFormatXMove() :
			MotionFileFormat("XMove", "XMove", "xmove,xml")
	{
	}

	FileFormatXMove::~FileFormatXMove()
	{
	}

	bool FileFormatXMove::Probe(const MotionFileHandler& /*context*/, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException)
	{
		// check for format
		if (!ProbeTextString(is, "<xmove", 1024))
			return false;

		// reset to beginning and check whether legacy read should be used
		is.seekg(0, std::ios::beg);
		FileFormatOptionsXMove xmove_options;
		xmove_options.LegacyCompoundNames = ProbeTextString(is, "<FormatID>CODAmotion_xmove</FormatID>", 1024);

		// store options
		readoptions = xmove_options.ToTree();

		// either way, will be ok to read this
		return true;
	}

  TreeValue* FileFormatXMove::Read(const MotionFileHandler& /*context*/, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException) 
	{
		// options
		FileFormatOptionsXMove xmove_options;
		if (readoptions)
		{
			xmove_options.FromTree(readoptions);
		}

		// load XML
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(is);
		if (!result)
		{
			throw MotionFileException(MotionFileException::formaterror, result.description());
		}

		std::auto_ptr<XMLReadingMachine> reader;
		if (xmove_options.LegacyCompoundNames)
		{
			// Support legacy version of format in which 
			// elements of time sequence and event groups structures have assumed types
			// and some naming conventions are different
			reader = std::auto_ptr<XMLReadingMachine>( new XMLReadingMachineLegacy );
		}
		else
		{
			// Standard XML reader
			reader = std::auto_ptr<XMLReadingMachine>( new XMLReadingMachine );
		}

		// find xmove node
		pugi::xml_node xmove_node = doc.child("xmove");

		// must have xmove node
		if (!xmove_node)
		{
			throw MotionFileException(MotionFileException::formaterror, "XML missing xmove section");
		}

		// parse XML tree
		TreeCompound* trial_object = TreeValueCast<TreeCompound>( reader->ReadValue(xmove_node) );

		// optionally replace any float32 info with float64
		if (xmove_options.ConvertBinaryFloat32)
		{
			for (size_t calclevel = 0; calclevel < trial_object->NumElements(); calclevel++)
			{
				TreeCompound* section = TreeValueCast<TreeCompound> ( trial_object->Node(calclevel)->Value() );
				ConvertListFloat32To64(section, "Sequences", TimeSequence::StructureName, memfactory);
				ConvertListFloat32To64(section, "EventGroups", EventGroup::StructureName, memfactory);
			}
		}

		return trial_object;

	}

	void FileFormatXMove::Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
	{
		// options
		FileFormatOptionsXMove xmove_options;
		xmove_options.FromTree(writeoptions);

		// format descriptor for audit trail
		FileFormatDescriptor descriptor;
		
		// fill with library info
		descriptor.ProgramName = context.ProgramName();
		descriptor.ProgramVersion = context.ProgramVersion();
		descriptor.LibraryName = context.LibraryName;
		descriptor.LibraryVersion = context.LibraryVersion;

		// choose format based on options
		std::auto_ptr<XMLWritingMachine> writer;
		if (xmove_options.LegacyCompoundNames)
		{
			descriptor.FormatID = "CODAmotion_xmove";
			writer = std::auto_ptr<XMLWritingMachine>(new XMLWritingMachineLegacy(os));
		}
		else
		{
			descriptor.FormatID = "CODAmotion_xmove2";
			writer = std::auto_ptr<XMLWritingMachine>(new XMLWritingMachine(os));
		}

		// compound object for trial
		const TreeCompound* trial_object = TreeValueCast<TreeCompound> ( contents );

		// optionally convert 64-bit floats to 32-bit
		std::auto_ptr<TreeCompound> contents_copy;
		if (xmove_options.ConvertBinaryFloat32)
		{
			if (trial_object)
			{
				// copy it
				contents_copy = std::auto_ptr<TreeCompound>( trial_object->NewBlank() );
				contents_copy->CopyFrom( contents );

				// set pointers to copy
				contents = trial_object = contents_copy.get();

				// remap binary data
				BinMemFactoryDefault memfactory;
				for (size_t calclevel = 0; calclevel < trial_object->NumElements(); calclevel++)
				{
					TreeCompound* section = TreeValueCast<TreeCompound> ( contents_copy.get()->Node(calclevel)->Value() );
					ConvertListFloat64To32(section, "Sequences", TimeSequence::StructureName, memfactory);
					ConvertListFloat64To32(section, "EventGroups", EventGroup::StructureName, memfactory);
				}
			}
		}

		os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

		// write contents
		const char* xmove_tag = "xmove";
		os << "<" << xmove_tag << ">\n";

		// format descriptor
		std::auto_ptr<TreeValue> descriptor_tree( descriptor.ToTree() );
		writer->WriteValue("FileFormat", descriptor_tree.get() );

		// all elements of trial, optionally excluding Calc section
		if (trial_object)
		{
			for (size_t element_index = 0; element_index < trial_object->NumElements(); element_index++)
			{
				const TreeCompoundNode* node = trial_object->Node(element_index);
				
				// optionally exclude calc
				if (xmove_options.ExcludeCalc && node->Name().compare(MEMBER_NAME(Trial::Calc)) == 0)
					continue;
				
				// otherwise write the node
				writer->WriteValue(node->Name(), node->Value());
			}
		}

		os << "</" << xmove_tag << ">\n";

	}
	
	void FileFormatXMove::ConvertListFloat32To64(TreeCompound* section, const char* listname, const char* structurename, BinMemFactory& memfactory)
	{
		if (section)
		{
			// get items
			const TreeList* ts_list_input = section->GetType<TreeList>(listname);
			if (ts_list_input)
			{					
				// convert
				TreeList* ts_list_output = ts_list_input->NewBlank();
				for (size_t iseq = 0; iseq < ts_list_input->NumElements(); iseq++)
				{
					const TreeValue* ts_input = ts_list_input->ElementArray()[iseq];
					TreeValue* ts_output = RichBinaryConvertFloat32To64(ts_input, structurename, memfactory);
					if (ts_output != NULL)
					{
						ts_list_output->Add( ts_output );
					}
				}
		
				// re-attach
				section->Remove(listname);
				section->Set(listname, ts_list_output);
			}
		}
	}

	void FileFormatXMove::ConvertListFloat64To32(TreeCompound* section, const char* listname, const char* structurename, BinMemFactory& memfactory)
	{
		if (section)
		{
			// get items
			const TreeList* ts_list_input = section->GetType<TreeList>(listname);
			if (ts_list_input)
			{
				// convert
				TreeList* ts_list_output = ts_list_input->NewBlank();
				for (size_t iseq = 0; iseq < ts_list_input->NumElements(); iseq++)
				{
					const TreeValue* ts_input = ts_list_input->ElementArray()[iseq];
					TreeValue* ts_output = RichBinaryConvertFloat64To32(ts_input, structurename, memfactory);
					if (ts_output != NULL)
					{
						ts_list_output->Add( ts_output );
					}
				}
		
				// re-attach
				section->Remove(listname);
				section->Set(listname, ts_list_output);
			}
		}
	}
}
