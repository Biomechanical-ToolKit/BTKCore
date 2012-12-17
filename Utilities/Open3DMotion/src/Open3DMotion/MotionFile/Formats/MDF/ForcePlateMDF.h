/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_PLATE_MDF_H_
#define _OPEN3DMOTION_FORCE_PLATE_MDF_H_

#include <map>
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"
#include "Open3DMotion/MotionFile/MotionFileException.h"
#include "Open3DMotion/Maths/Vector3.h"

namespace Open3DMotion
{
	class ForcePlateMDF : public ForcePlate
  {
	public:
		// MDF files support plate orentation in these discrete steps:
		enum Orientation
		{
			orient_invalid = -1,
			orient000 = 0,
			orient090 = 1,
			orient180 = 2,
			orient270 = 3
		};

	public:
	
		// Make empty
    ForcePlateMDF();

		// Copy constructor
    ForcePlateMDF(const ForcePlate& src);

	public:

		// Get number of plates from MDF structure
		static size_t NumPlates(
			const std::map<size_t, std::vector< std::vector<UInt8> >, std::less<size_t> >& data,
			const std::map<size_t, size_t, std::less<size_t> >& elementsize);

	public:

		// Parse from MDF-style file structure and plate index
		void ParseMDF(
			std::map<size_t, std::vector< std::vector<UInt8> >, std::less<size_t> >& data,
			std::map<size_t, size_t, std::less<size_t> >& elementsize,
			size_t iplate) throw(MotionFileException);

		float MDFDefaultSaveRes(size_t ichannel) const;

    const char* MDFChannelUnits(size_t ichannel) const;

		/** Summary
				Deals with sign conversion from data stored on tree to data stored in MDF.

				Description
				Used in conjunction with RuntimeChannelToMDFChannel this rearranges channels
				as required.

				AMTI Plate at ??? degrees
				                      1       2       3       4       5       6       7       8
				Runtime Channel:      Fx      Fy      Fz      Mx      My      Mz      P0      P1
				MDF:                 -Fy     -Fx      Fz   .001My   .001Mx  .001Mz    P0      P1


		*/

		double MDFChannelScale(size_t ichannel) const;

		size_t MDFChannelToRuntimeChannel(size_t mdfchannel) const;

		size_t RuntimeChannelToMDFChannel(size_t runchannel) const;

		UInt8 MDFPlateID() const throw(MotionFileException);

		void MDFSensorConstants(Int16* w) const;

		// deduce MDF orientation from Outline (may be invalid)
		Orientation MDFOrientation() const;

		// make MDF-compatible outline to match ChannelScale and RuntimeChanneltoMDFChannel
		// returns true if made ok, false if invalid orientation
		bool MDFOutline(std::vector<Vector3>& mdfoutline) const;

	public:

    enum plateType
    {
      type_Kistler_9821B = 1,
      type_Kistler_9826 = 2,
      type_Kistler_9821C_1 = 4,
      type_Kistler_9821C_2 = 5,
      type_Kistler_9827 = 6,
      type_Kistler_9261 = 7,
      type_AMTI_BP2416 = 10,
      type_AMTI_LG6 = 11,
			type_AMTI_OR6_7 = 12,
      type_AMTI_AccuSway = 20,
			type_AMTI_AccuGait=21
    };

/* 
  public:
    BYTE id;
    Vector3 sensor;
    TOutline outline;
		bool have_CoP_correction;
		double CoP_correction[2][6]; 
*/

	protected:

		bool HasAMTIChannelScheme() const;

		void DefaultSensorConstants(UInt8 id);

    void DefaultOutline();

		void ParseMDFSensorConstants(const Int16* w);

		static double RoundSensorConst(double k);

		static const char* IDtoModel(UInt8 id) throw(MotionFileException);

		static const char* IDtoType(UInt8 id) throw(MotionFileException);
	};
}

#endif
