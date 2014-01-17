/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BIN_ITER_H_
#define _ORMPP_BIN_ITER_H_

#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"

namespace Open3DMotion
{
	template<typename DataType> void ByteIncrement(DataType*& ptr, size_t bytes)
	{ ptr = reinterpret_cast<DataType*>( (reinterpret_cast<const UInt8*>(ptr) + bytes) ); }

	template<typename DataType> void ByteDecrement(DataType*& ptr, size_t bytes)
	{ ptr = reinterpret_cast<DataType*>( (reinterpret_cast<const UInt8*>(ptr) - bytes) ); }

	class BinIterBase
	{
	public:
		BinIterBase(const RichBinary& bin) :
			frameindex(0UL)
		{
			framesize = bin.DataStructure().TotalBytes();
			numframes = bin.DataSizeBytes() / framesize;
		}
		
    virtual ~BinIterBase() {};

	protected:
		template<typename DataType> static const DataType* GetFieldPointer(const RichBinary& bin, const char* fieldname, size_t dimension) throw(NoSuchFieldException)
		{
			// get spec and relative offset - will throw exception if not found
			const BinaryFieldSpec* spec(NULL);
			size_t offset(0);
			bin.DataStructure().GetFieldOffset(spec, offset, fieldname);

			// ensure correct type
			const char* expected_field_type = BinaryFieldTypeString<DataType> ();
			if ((expected_field_type == NULL) || (spec->Type.Value().compare(expected_field_type) != 0))
				throw NoSuchFieldException(fieldname, NoSuchFieldException::wrongtype);

			// ensure dimension suitable
			if ((dimension*sizeof(DataType)) > size_t(spec->Bytes.Value()))
				throw NoSuchFieldException(fieldname, NoSuchFieldException::wrongdimension);

			// do pointer arithmetic to get answer
			return reinterpret_cast<const DataType*> ( bin.Data() + offset );
		}

		template<typename DataType> static const DataType* GetOptionalFieldPointer(size_t& dimension, const RichBinary& bin, const char* fieldname)
		{
			try
			{
				// get spec and relative offset - will throw exception if not found
				const BinaryFieldSpec* spec(NULL);
				size_t offset(0);
				bin.DataStructure().GetFieldOffset(spec, offset, fieldname);

				// ensure correct type
				const char* expected_field_type = BinaryFieldTypeString<DataType> ();
				if ((expected_field_type != NULL) && (spec->Type.Value().compare(expected_field_type) == 0))
				{
					// store dimension
					dimension = spec->Dimension;

					// compute offset and return
					return reinterpret_cast<const DataType*> ( bin.Data() + offset );
				}
			}
			catch (const NoSuchFieldException&)
			{
				// ignore exceptions - just return NULL
			}

			// initialise dimension to zero
			dimension = 0UL;
			return NULL;
		}

	public:

		bool HasFrame() const
		{
			return (frameindex < numframes);
		}
		
		size_t NumFrames() const
		{ return numframes; }

		size_t FrameIndex() const
		{ return frameindex; }

	public:

		// Increment ptrs
		void Next()
		{
			IncrementPointers(framesize);
			frameindex++;
		}

		// Decrement ptrs
		void Prev()
		{
			DecrementPointers(framesize);
			frameindex--;
		}

		// Increment ptrs N times
		void NextN(size_t n)
		{
			IncrementPointers(n*framesize);
			frameindex += n;
		}

		// Decrement ptrs N times
		void PrevN(size_t n)
		{
			DecrementPointers(n*framesize);
			frameindex -= n;
		}

		// Seek to location
		void SeekTo(size_t index)
		{
			if (index < frameindex)
			{
				DecrementPointers(framesize*(frameindex - index));
			}
			else
			{
				IncrementPointers(framesize*(index - frameindex));
			}

			frameindex = index;
		}

	protected:

		virtual void IncrementPointers(size_t offset) = 0;

		virtual void DecrementPointers(size_t offset) = 0;

	protected:

		// size for ptr increments
		size_t framesize;

		// total frames
		size_t numframes;

		// current ptr location w.r.t. zero
		size_t frameindex;
	};

	template<typename TypeA> class BinConstIter1 : public BinIterBase
	{
	public:
		BinConstIter1(const RichBinary& bin, const char* fieldA, size_t dimA) throw(NoSuchFieldException) :
			BinIterBase(bin),
			a( GetFieldPointer<TypeA> (bin, fieldA, dimA) )
		{
		}

	public:
		const TypeA* ValuePtr0() const
		{ return a; }

	protected:

		virtual void IncrementPointers(size_t bytes)
		{
			ByteIncrement<const TypeA>(a, bytes);
		}

		virtual void DecrementPointers(size_t bytes)
		{
			ByteDecrement<const TypeA>(a, bytes);
		}

	protected:

		// underlying item
		const TypeA* a;
	};

	template<typename TypeA> class BinIter1 : public BinConstIter1<TypeA>
	{
	public:
		BinIter1(RichBinary& bin, const char* fieldA, size_t dimA)  throw(NoSuchFieldException) :
			BinConstIter1<TypeA>(bin, fieldA, dimA)
		{
		}

	public:
		TypeA* ValuePtr0() const
		{ return const_cast<TypeA*>( BinConstIter1<TypeA>::a ); }
	};


	template<typename TypeA, typename TypeB> class BinConstIter2 : public BinConstIter1<TypeA>
	{
	public:
		BinConstIter2(const RichBinary& bin, const char* fieldA, size_t dimA, const char* fieldB, size_t dimB) throw(NoSuchFieldException) :
				BinConstIter1<TypeA> (bin, fieldA, dimA),
        b( BinIterBase::GetFieldPointer<TypeB>(bin, fieldB, dimB) )
		{
		}

	public:

		const TypeB* ValuePtr1() const
		{ return b; }

	protected:

		virtual void IncrementPointers(size_t bytes)
		{
			BinConstIter1<TypeA>::IncrementPointers(bytes);
			ByteIncrement<const TypeB>(b, bytes);
		}

		virtual void DecrementPointers(size_t bytes)
		{
			BinConstIter1<TypeA>::DecrementPointers(bytes);
			ByteDecrement<const TypeB>(b, bytes);
		}

	protected:
		const TypeB* b;
	};


	template<typename TypeA, typename TypeB> class BinIter2 : public BinConstIter2<TypeA, TypeB>
	{
	public:
		BinIter2(const RichBinary& bin, const char* fieldA, size_t dimA, const char* fieldB, size_t dimB) throw(NoSuchFieldException) :
				 BinConstIter2<TypeA, TypeB>(bin, fieldA, dimA, fieldB, dimB)
		{
		}

	public:
		TypeA* ValuePtr0() const
		{ return const_cast<TypeA*>( BinConstIter1<TypeA>::a ); }

		TypeB* ValuePtr1() const
		{ return const_cast<TypeB*>( BinConstIter2<TypeA, TypeB>::b ); }
	};

	template<typename TypeA, typename TypeB, typename TypeC> class BinConstIter3 : public BinConstIter2<TypeA, TypeB>
	{
	public:
		BinConstIter3(const RichBinary& bin, const char* fieldA, size_t dimA, const char* fieldB, size_t dimB, const char* fieldC, size_t dimC) throw(NoSuchFieldException) :
				BinConstIter2<TypeA, TypeB> (bin, fieldA, dimA, fieldB, dimB),
        c( BinIterBase::GetFieldPointer<TypeC>(bin, fieldC, dimC) )
		{
		}

	public:

		const TypeC* ValuePtr2() const
		{ return c; }

	protected:

		virtual void IncrementPointers(size_t bytes)
		{
			BinConstIter2<TypeA, TypeB>::IncrementPointers(bytes);
			ByteIncrement<const TypeC>(c, bytes);
		}

		virtual void DecrementPointers(size_t bytes)
		{
			BinConstIter2<TypeA, TypeB>::DecrementPointers(bytes);
			ByteDecrement<const TypeC>(c, bytes);
		}

	protected:
		const TypeC* c;
	};


	template<typename TypeA, typename TypeB, typename TypeC> class BinIter3 : public BinConstIter3<TypeA, TypeB, TypeC>
	{
	public:
		BinIter3(const RichBinary& bin, const char* fieldA, size_t dimA, const char* fieldB, size_t dimB, const char* fieldC, size_t dimC) throw(NoSuchFieldException) :
				 BinConstIter3<TypeA, TypeB, TypeC>(bin, fieldA, dimA, fieldB, dimB, fieldC, dimC)
		{
		}

	public:
		TypeA* ValuePtr0() const
		{ return const_cast<TypeA*>( BinConstIter1<TypeA>::a ); }

		TypeB* ValuePtr1() const
		{ return const_cast<TypeB*>( BinConstIter2<TypeA, TypeB>::b ); }

		TypeC* ValuePtr2() const
		{ return const_cast<TypeC*>( BinConstIter3<TypeA, TypeB, TypeC>::c ); }
	};
}

#endif