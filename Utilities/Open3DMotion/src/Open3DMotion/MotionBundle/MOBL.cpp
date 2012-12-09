/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionBundle/MOBL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONGZStreamReader.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONStreamReader.h"

namespace Open3DMotion
{
	MOBLReadOptions::MOBLReadOptions()
	{
		REGISTER_MEMBER(UseGZIP);
		REGISTER_MEMBER(FormatVersion);
	}

	bool MOBLFormat::Probe(MOBLReadOptions& readoptions, std::istream& is) throw(MotionFileException)
	{
		/*
	 * The signature block is stored at the beginning of a motion bundle file so the file
	 * format can easily be identified.  The signature is the bson encoding of one of
	 * the following JSON statements:
	 * 
	 * { "CODABUNDLE" : 1 }
	 * { "CODAGZNDLE" : 1 }
	 * 
	 * This means the data structure is:
	 * 
	 * UInt32 Size          : 21  (total size of this signature block as little-endian)
	 * UInt8  DataType      : 16 (0x10, means a BSON cstring text key with corresponding int32 value)
	 * UInt8  FormatID[10]  : "CODABUNDLE" or "CODAGZUNDLE" as ASCII
	 * UInt8  FormatIDTerm  : 0 (BSON terminator for the format id string)
	 * UInt32 FormatVersion : 1
	 * UInt8  SignatureTerm : 0 (BSON terminator for the signature block)
	 */

		// init to zero
		readoptions.FormatVersion = 0;

		// read header
		UInt32 size(0);
		char formatid[11];
		is.read((char*)&size, 4);
		if (size == 21)
		{
			UInt8 datatype(0);
			is.read((char*)&datatype, 1);
			if (datatype == 0x10)
			{
				is.read(formatid, 11);
				is.read((char*)&readoptions.FormatVersion.Value(), 4);
			}
		}

		// must have valid format
		if (readoptions.FormatVersion < 1)
			return false;

		// must be recognised format label
		if (strncmp(formatid, "CODABUNDLE", 11) == 0)
		{
			readoptions.UseGZIP = false;
		}
		else if (strncmp(formatid, "CODAGZNDLE", 11) == 0)
		{
			readoptions.UseGZIP = true;
		}
		else
		{
			return false;
		}

		return true;
	}

	MOBLFormatReader* MOBLFormat::ReadBegin(std::istream* is, const MOBLReadOptions& readoptions)
	{
		return new MOBLFormatReader(is, readoptions);
	}

	MOBLFormatReader::MOBLFormatReader(std::istream* _is, const MOBLReadOptions& _readoptions) :
		is(_is),
		readoptions(_readoptions),
		reader(NULL),
		num_docs_read(0UL)
	{
	}

	MOBLFormatReader::~MOBLFormatReader()
	{
		ClearReader();
	}

	void MOBLFormatReader::ClearReader()
	{
		if (reader != NULL)
		{
			delete reader;
			reader = NULL;
		}
	}

	bool MOBLFormatReader::PositionReader(UInt32 doc_index_needed)  throw(MotionFileException)
	{
		// must reset reader if we've already read past this index
		// or if no reader yet created
		if ((num_docs_read > doc_index_needed) || (reader == NULL))
		{
			// remove previous reader if any
			ClearReader();

			// clear error bits
			is->clear();

			// skip header (always uncompressed)
			is->seekg(21, std::ios::beg);

			// use appropriate decoder
			if (readoptions.UseGZIP)
				reader = new BSONGZStreamReader(*is);
			else
				reader = new BSONStreamReader(*is);

			// always ignore first 4 bytes of binary blobs for MOBL compatibility
			reader->BinaryMOBLCompatible() = true;

			// reset docs read
			num_docs_read = 0;
		}

		// skip to trial
		try
		{
			while (num_docs_read < doc_index_needed)
			{
				UInt32 docsize(0);
				reader->ReadBinary(&docsize, 4);
				reader->SkipBytes(docsize-4);
				++num_docs_read;
			}
		}
		catch (const BSONReadException& e)
		{
			throw MotionFileException(MotionFileException::formaterror, e.message.c_str());
		}

		return reader->HaveMore();
	}

	UInt32 MOBLFormatReader::TrialCount()  throw(MotionFileException)
	{
		UInt32 count(0);
		while (PositionReader(count+1))
			count++;
		return count;
	}

	TreeValue* MOBLFormatReader::ReadTrial(UInt32 index) throw(MotionFileException)
	{
		// move to required trial
		UInt32 doc_index_needed = index + 1;
		if (!PositionReader(doc_index_needed))
			throw MotionFileException(MotionFileException::formaterror, "invalid trial index");

		// auto ptr so it will get released on exception thrown
		std::auto_ptr<TreeCompound> MOBLresult( new TreeCompound );

		// read
		try
		{
			reader->ReadDocument(*MOBLresult);
		}
		catch (const BSONReadException& e)
		{
			throw MotionFileException(MotionFileException::formaterror, e.message.c_str());
		}

		// successful read means now at next doc
		++num_docs_read;

		// the data is actually in 'd' subsections
		const TreeCompound* MOBLAcq = MOBLresult->GetType<TreeCompound>("Acq");
		const TreeCompound* MOBLAcq_d = MOBLAcq ? MOBLAcq->GetType<TreeCompound>("d") : NULL;
		const TreeCompound* MOBLUserInput = MOBLresult->GetType<TreeCompound>("UserInput");
		const TreeCompound* MOBLUserInput_d = MOBLUserInput ? MOBLUserInput->GetType<TreeCompound>("d") : NULL;

		// copy those parts (should do shallow copy for time sequences anyway)
		TreeCompound* Acq = new TreeCompound;
		TreeCompound* UserInput = new TreeCompound;
		Acq->CopyFrom(MOBLAcq_d);
		UserInput->CopyFrom(MOBLUserInput_d);

		// make new item
		TreeCompound* result = new TreeCompound;
		result->Set("Acq", Acq);
		result->Set("UserInput", UserInput);
		return result;
	}

}
