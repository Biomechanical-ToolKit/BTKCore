/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

namespace Open3DMotion
{
	class TSFactoryC3D : public TSFactoryOccValue
	{
	public:
		static const char fieldname_confidence[];
		static const char fieldname_c3dflag[];

	public:
		TSFactoryC3D();
	};

	class TSC3DConstIter : public BinConstIter2<double, UInt8>
	{
	public:
			TSC3DConstIter(const TimeSequence& ts) throw(NoSuchFieldException) :
				BinConstIter2<double, UInt8>(ts, 
					TSFactoryValue::fieldname_value, 3, 
					TSFactoryOccValue::fieldname_occluded, 1)
		{
			confidence = GetOptionalFieldPointer<double> (confidence_dimension, ts, TSFactoryC3D::fieldname_confidence);
			cameraflag = GetOptionalFieldPointer<UInt8> (cameraflag_dimension, ts, TSFactoryC3D::fieldname_c3dflag);
		}

	public:
		const double* Value() const
		{ return ValuePtr0(); }

		const UInt8& Occluded() const
		{ return *ValuePtr1(); }

		const double* Confidence() const
		{ return confidence; }

		const UInt8 CameraFlag() const
		{ return (cameraflag != NULL) ? *cameraflag : 0UL; }

		size_t ConfidenceDimension() const
		{ return confidence_dimension; } 

		bool HasCameraFlag() const
		{ return (cameraflag != NULL); }

	protected:

		virtual void IncrementPointers(size_t bytes)
		{
			BinConstIter2<double, UInt8>::IncrementPointers(bytes);
			if (confidence)
				ByteIncrement<const double>(confidence, bytes);
			if (cameraflag)
				ByteIncrement<const UInt8>(cameraflag, bytes);
		}

		virtual void DecrementPointers(size_t bytes)
		{
			BinConstIter2<double, UInt8>::DecrementPointers(bytes);
			if (confidence)
				ByteDecrement<const double>(confidence, bytes);
			if (cameraflag)
				ByteDecrement<const UInt8>(cameraflag, bytes);
		}

	protected:
		size_t confidence_dimension;
		size_t cameraflag_dimension;
		const double* confidence;
		const UInt8* cameraflag;
	};

	class TSC3DIter : public TSC3DConstIter
	{
	public:
		TSC3DIter(TimeSequence& ts) throw(NoSuchFieldException) :
				TSC3DConstIter(ts)
		{
		}

	public:
		double* Value() const
		{ return const_cast<double*>( TSC3DConstIter::Value() ); }

		UInt8& Occluded() const
		{ return const_cast<UInt8&>( TSC3DConstIter::Occluded() ); }

		double* Confidence() const
		{ return const_cast<double*>( confidence ); }

		UInt8& CameraFlag() const
		{ return *const_cast<UInt8*>( cameraflag ); }
	};

}
