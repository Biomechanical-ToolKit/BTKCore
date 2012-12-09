/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"

namespace Open3DMotion
{
	class TSFactory
	{
	protected:
		TSFactory()
		{
		}

	public:
		TimeSequence* New(const TimeRange& tr, BinMemFactory& memfactory) const;

	protected:
		void AddField(const BinaryFieldSpec& field);

	private:
		std::vector<BinaryFieldSpec> layout;
	};

	class TSFactoryValue : public TSFactory
	{
	public:
		static const char fieldname_value[];

	public:
		TSFactoryValue(Int32 dimension);
	};

	class TSFactoryOccValue : public TSFactoryValue
	{
	public:
		static const char fieldname_occluded[];

	public:
		TSFactoryOccValue(Int32 dimension);
	};

	class TSScalarConstIter : public BinConstIter1<double>
	{
	public:
		TSScalarConstIter(const TimeSequence& ts) throw(NoSuchFieldException)  :
				BinConstIter1<double>(ts, TSFactoryValue::fieldname_value, 1)
		{
		}

	public:
		const double& Value() const
		{ return *ValuePtr0(); }
	};

	class TSScalarIter : public BinIter1<double>
	{
	public:
		TSScalarIter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter1<double>(ts, TSFactoryValue::fieldname_value, 1)
		{
		}

	public:
		double& Value() const
		{ return *ValuePtr0(); }
	};

	class TSVector3ConstIter : public BinConstIter1<double>
	{
	public:
			TSVector3ConstIter(const TimeSequence& ts) throw(NoSuchFieldException) :
				BinConstIter1<double>(ts, TSFactoryValue::fieldname_value, 3)
		{
		}

	public:
		const double* Value() const
		{ return ValuePtr0(); }
	};

	class TSVector3Iter : public BinIter1<double>
	{
	public:
		TSVector3Iter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter1<double>(ts, TSFactoryValue::fieldname_value, 3)
		{
		}

	public:
		double* Value() const
		{ return ValuePtr0(); }
	};

	class TSOccVector3ConstIter : public BinConstIter2<double, UInt8>
	{
	public:
			TSOccVector3ConstIter(const TimeSequence& ts) throw(NoSuchFieldException) :
				BinConstIter2<double, UInt8>(ts, TSFactoryValue::fieldname_value, 3, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		const double* Value() const
		{ return ValuePtr0(); }

		const UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccVector3Iter : public BinIter2<double, UInt8>
	{
	public:
		TSOccVector3Iter(TimeSequence& ts) throw(NoSuchFieldException)  :
			BinIter2<double, UInt8>(ts, TSFactoryValue::fieldname_value, 3, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		double* Value() const
		{ return ValuePtr0(); }

		UInt8& Occluded() const
		{ return *ValuePtr1(); }
	};

	class TSOccConstIter : public BinConstIter1<UInt8>
	{
	public:
		TSOccConstIter(const TimeSequence& ts) throw(NoSuchFieldException)  :
				BinConstIter1<UInt8>(ts, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		const UInt8& Occluded() const
		{ return *ValuePtr0(); }
	};

	class TSOccIter : public BinIter1<UInt8>
	{
	public:
		TSOccIter(TimeSequence& ts) throw(NoSuchFieldException)  :
				BinIter1<UInt8>(ts, TSFactoryOccValue::fieldname_occluded, 1)
		{
		}

	public:
		UInt8& Occluded() const
		{ return *ValuePtr0(); }
	};

};
