/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_TRIAL_H_
#define _OPEN3DMOTION_TRIAL_H_

#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"

namespace Open3DMotion
{
	class TrialSection : public MapCompound
	{
	public:
		TrialSection() :
			TimeSequences("Sequence"),
			EventGroups("EventGroup")
		{
			Register("Sequences", &TimeSequences);
			REGISTER_MEMBER(EventGroups);
		}

	public:
		
		void GetTSGroup(std::vector<const TimeSequence*>& tsgroup, const char* groupname) const;

		bool GetTSIndexByHardwareID(size_t& index, const char* groupname, Int32 hardwareID) const;

		const TimeSequence* GetTS(const char* groupname, const char* channelname) const;

		const EventGroup* GetEventGroup(const char* name) const;

	public:
		MapArrayCompound<TimeSequence> TimeSequences;
		MapArrayCompound<EventGroup> EventGroups;
	};

	class MeasurementSystemSpec : public MapCompound
	{
	public:
		MeasurementSystemSpec()
		{
			REGISTER_MEMBER(Name);
			REGISTER_MEMBER(Company);
			REGISTER_MEMBER(SoftwareName);
			REGISTER_MEMBER(SoftwareVersion);
			REGISTER_MEMBER(DataServerName);
			REGISTER_MEMBER(DataServerVersion);
		}

	public:
		MapOptionalString Name;
		MapOptionalString Company;
		MapOptionalString SoftwareName;
		MapOptionalString SoftwareVersion;
		MapOptionalString DataServerName;
		MapOptionalString DataServerVersion;
	};

	class TrialDate : public MapCompound
	{
	public:
		TrialDate()
		{
			REGISTER_MEMBER(Year);
			REGISTER_MEMBER(Month);
			REGISTER_MEMBER(Day);
		}

	public:
		MapOptionalInt32 Year;
		MapOptionalInt32 Month;
		MapOptionalInt32 Day;
	};

	class TrialVideoCameraCalibration : public MapCompound
	{
	public:
		TrialVideoCameraCalibration() :
			ProjectionMatrix("p")
		{
			REGISTER_MEMBER(ProjectionMatrix);
		}

	public:
		MapArrayFloat64 ProjectionMatrix;
	};

	class TrialVideoCameraFileSpec : public MapCompound
	{
	public:
		TrialVideoCameraFileSpec()
		{
			REGISTER_MEMBER(VideoFilePattern);
		}

	public:
		MapOptionalString VideoFilePattern;
	};

	class TrialVideoCamera : public MapCompound
	{
	public:
		TrialVideoCamera()
		{
			REGISTER_MEMBER(Calibration);
			REGISTER_MEMBER(VideoSpec);
		}

	public:
		MapOptional<TrialVideoCameraCalibration> Calibration;
		MapOptional<TrialVideoCameraFileSpec> VideoSpec;
	};

	class TrialSectionAcq : public TrialSection
	{
	public:
		const static char TSGroupMarker[];
		const static char TSGroupAnalog[];

	public:
		TrialSectionAcq() :
			ForcePlates("ForcePlate"),
			Video("Camera")
		{
			REGISTER_MEMBER(Date);
			REGISTER_MEMBER(MeasurementSystem);
			REGISTER_MEMBER(ForcePlates);
			REGISTER_MEMBER(Video);
		}

	public:
		MapOptional<TrialDate> Date;
		MapOptional<MeasurementSystemSpec> MeasurementSystem;
		MapArrayCompound<ForcePlate> ForcePlates;
		MapArrayCompound<TrialVideoCamera> Video;
	};

	class TrialSubject : public MapCompound
	{
	public:
		TrialSubject()
		{
			REGISTER_MEMBER(ID);
			REGISTER_MEMBER(Classification);
			REGISTER_MEMBER(Gender);
			REGISTER_MEMBER(DateOfBirth);			
			REGISTER_MEMBER(Age);
			REGISTER_MEMBER(Height);
			REGISTER_MEMBER(Weight);
			
			REGISTER_MEMBER(PelvicWidth);
			REGISTER_MEMBER(PelvicDepth);
			REGISTER_MEMBER(LKneeWidth);
			REGISTER_MEMBER(LAnkleWidth);
			REGISTER_MEMBER(RKneeWidth);
			REGISTER_MEMBER(RAnkleWidth);
			REGISTER_MEMBER(SacralOffset);

			REGISTER_MEMBER(LThighLength);
			REGISTER_MEMBER(LShankLength);
			REGISTER_MEMBER(LFootLength);
			REGISTER_MEMBER(RThighLength);
			REGISTER_MEMBER(RShankLength);
			REGISTER_MEMBER(RFootLength);

			REGISTER_MEMBER(LThighMass);
			REGISTER_MEMBER(LShankMass);
			REGISTER_MEMBER(LFootMass);
			REGISTER_MEMBER(RThighMass);
			REGISTER_MEMBER(RShankMass);
			REGISTER_MEMBER(RFootMass);

			REGISTER_MEMBER(RadGyr_LThigh_X);
			REGISTER_MEMBER(RadGyr_LThigh_Y);
			REGISTER_MEMBER(RadGyr_LThigh_Z);
			REGISTER_MEMBER(RadGyr_LShank_X);
			REGISTER_MEMBER(RadGyr_LShank_Y);
			REGISTER_MEMBER(RadGyr_LShank_Z);
			REGISTER_MEMBER(RadGyr_LFoot_X);
			REGISTER_MEMBER(RadGyr_LFoot_Y);
			REGISTER_MEMBER(RadGyr_LFoot_Z);

			REGISTER_MEMBER(RadGyr_RThigh_X);
			REGISTER_MEMBER(RadGyr_RThigh_Y);
			REGISTER_MEMBER(RadGyr_RThigh_Z);
			REGISTER_MEMBER(RadGyr_RShank_X);
			REGISTER_MEMBER(RadGyr_RShank_Y);
			REGISTER_MEMBER(RadGyr_RShank_Z);
			REGISTER_MEMBER(RadGyr_RFoot_X);
			REGISTER_MEMBER(RadGyr_RFoot_Y);
			REGISTER_MEMBER(RadGyr_RFoot_Z);
		}

	public:

		MapOptionalString ID;
		MapOptionalString Classification;
		MapOptionalString Gender;
		MapOptional<TrialDate> DateOfBirth;		
		MapOptionalInt32 Age;
		MapOptionalFloat64 Height;
		MapOptionalFloat64 Weight;
		
		MapOptionalFloat64 PelvicWidth;
		MapOptionalFloat64 PelvicDepth;
		MapOptionalFloat64 LKneeWidth;
		MapOptionalFloat64 LAnkleWidth;
		MapOptionalFloat64 RKneeWidth;
		MapOptionalFloat64 RAnkleWidth;
		MapOptionalFloat64 SacralOffset;

		MapOptionalFloat64 LThighLength;
		MapOptionalFloat64 LShankLength;
		MapOptionalFloat64 LFootLength;
		MapOptionalFloat64 RThighLength;
		MapOptionalFloat64 RShankLength;
		MapOptionalFloat64 RFootLength;

		MapOptionalFloat64 LThighMass;
		MapOptionalFloat64 LShankMass;
		MapOptionalFloat64 LFootMass;
		MapOptionalFloat64 RThighMass;
		MapOptionalFloat64 RShankMass;
		MapOptionalFloat64 RFootMass;
		
		MapOptionalFloat64 RadGyr_LThigh_X;
		MapOptionalFloat64 RadGyr_LThigh_Y;
		MapOptionalFloat64 RadGyr_LThigh_Z;

		MapOptionalFloat64 RadGyr_LShank_X;
		MapOptionalFloat64 RadGyr_LShank_Y;
		MapOptionalFloat64 RadGyr_LShank_Z;

		MapOptionalFloat64 RadGyr_LFoot_X;
		MapOptionalFloat64 RadGyr_LFoot_Y;
		MapOptionalFloat64 RadGyr_LFoot_Z;

		MapOptionalFloat64 RadGyr_RThigh_X;
		MapOptionalFloat64 RadGyr_RThigh_Y;
		MapOptionalFloat64 RadGyr_RThigh_Z;

		MapOptionalFloat64 RadGyr_RShank_X;
		MapOptionalFloat64 RadGyr_RShank_Y;
		MapOptionalFloat64 RadGyr_RShank_Z;

		MapOptionalFloat64 RadGyr_RFoot_X;
		MapOptionalFloat64 RadGyr_RFoot_Y;
		MapOptionalFloat64 RadGyr_RFoot_Z;
	};

	class TrialSectionUserInput : public TrialSection
	{
	public:
		TrialSectionUserInput()
		{
			REGISTER_MEMBER(Comment);
			REGISTER_MEMBER(RepLabel);
			REGISTER_MEMBER(TrialCondition);
			REGISTER_MEMBER(Subject);
		}

	public:
		MapOptionalString Comment;
		MapOptionalString RepLabel;
		MapOptionalString TrialCondition;
		MapOptional<TrialSubject> Subject;
	};

	class FileFormatInformation : public MapCompound
	{
	public:
		FileFormatInformation()
		{
			REGISTER_MEMBER(ID);
		}

	public:
		MapString ID;
	};

	class Trial : public MapCompound
	{
	public:
		Trial()
		{
			REGISTER_MEMBER(Acq);
			REGISTER_MEMBER(UserInput);
			REGISTER_MEMBER(Calc);
		}

	public:
		FileFormatInformation FileFormat;
		TrialSectionAcq Acq;
		MapOptional<TrialSectionUserInput> UserInput;
		MapOptional<TrialSection> Calc;
	};
}
#endif