/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "ForcePlateMDF.h"
#include "MDFVarTypes.h"
#include "Open3DMotion/Maths/LinearSolve3.h"
#include "Open3DMotion/Maths/RigidTransform3.h"
#include <math.h>

namespace Open3DMotion
{
  ForcePlateMDF::ForcePlateMDF()
  {
  }

	ForcePlateMDF::ForcePlateMDF(const ForcePlate& src) :
		ForcePlate(src)
	{
	}

	size_t ForcePlateMDF::NumPlates(
			const std::map<size_t, std::vector< std::vector<UInt8> >, std::less<size_t> >& data,
			const std::map<size_t, size_t, std::less<size_t> >& elementsize)
	{
		std::map<size_t, size_t, std::less<size_t> >::const_iterator sizeinfo = elementsize.find( VAR_ANALOGUE_FORCE );
		size_t force_element_size = (sizeinfo != elementsize.end()) ? sizeinfo->second : 0;

		// allow zero here - should this really be allowed??
		if (force_element_size == 0)
			return 0UL;

		// if non-zero should always indicate 2 bytes per value
		if (force_element_size != 2)
      throw(MotionFileException(MotionFileException::formaterror,"Unexpected element size for force data"));
    
    // see what force plate descriptors are available
		size_t num_corners = data.find(VAR_FORCE_PLATE_POSITION)->second.size();
    size_t num_sensor_offsets = data.find(VAR_FORCE_PLATE_CONSTANTS)->second.size();
		size_t num_force_types = data.find(VAR_FORCE_PLATE_FLAGS)->second.size();

    // establish number of plates
    size_t num_plates(0);
    if (num_force_types)
    {
			if (elementsize.find(VAR_FORCE_PLATE_FLAGS)->second != 2)
        throw(MotionFileException(MotionFileException::formaterror,"Unexpected element size for force plate flags"));
      num_plates = num_force_types;
    }
    else if (num_corners)
    {
      if (num_corners % 4 != 0)
        throw(MotionFileException(MotionFileException::formaterror,"Error in force plate corners"));
      num_plates = num_corners / 4;
    }
    else
    {
      // default force plate types for all plates
			size_t num_force = data.find(VAR_ANALOGUE_FORCE)->second.size();
      if (num_force % 8 != 0)
        throw(MotionFileException(MotionFileException::formaterror,"No force plate info was given for non-standard plate"));
      num_plates = num_force / 8;
    }

    // check appropriate number of corners etc.
    if (num_corners && num_corners != (4*num_plates))
      throw(MotionFileException(MotionFileException::formaterror,"4 corners per force plate not present"));
    if (num_sensor_offsets && num_sensor_offsets != num_plates)
      throw(MotionFileException(MotionFileException::formaterror,"3 sensor offsets per force plate not present"));

		// return number of plates
		return num_plates;
	}

	void ForcePlateMDF::ParseMDF(
			std::map<size_t, std::vector< std::vector<UInt8> >, std::less<size_t> >& data,
			std::map<size_t, size_t, std::less<size_t> >& /*elementsize*/,
			size_t iplate) throw(MotionFileException)
  {
		// MDF plate ID - init to zero (invalid)
		UInt8 id(0);

		// read plate type - may be 0 if file with no flags was re-saved using CODAmotion Analysis
		if (iplate < data[VAR_FORCE_PLATE_FLAGS].size() &&
				data[VAR_FORCE_PLATE_FLAGS][iplate].size() >= 2)
    {
      UInt16 wType = *(UInt16*)(&(data[VAR_FORCE_PLATE_FLAGS][iplate][0]));
      id = wType >> 8;
    }

		// if zero or not specified, default to Kistler_9821B
		if (id == 0)
			id = (UInt8)ForcePlateMDF::type_Kistler_9821B;

		// get plate type and model from ID - will generate exception if not recognised or supported
		Type = IDtoType(id);
		Model = IDtoModel(id);

		// set default basic params
    DefaultOutline();
    DefaultSensorConstants(id);

    // custom sensor constants 
    // - defaults will be left if not available
    if (iplate < data[VAR_FORCE_PLATE_CONSTANTS].size() &&
				data[VAR_FORCE_PLATE_CONSTANTS][iplate].size() >= 6)
    {
      ParseMDFSensorConstants((const Int16*)&data[VAR_FORCE_PLATE_CONSTANTS][iplate][0]);
    }

    // custom outline
    if ((4*iplate+3) < data[VAR_FORCE_PLATE_POSITION].size())
    {
      for (size_t i = 0; i < 4; i++)
      {
				size_t nelements = data[VAR_FORCE_PLATE_POSITION][4*iplate+i].size();
        if (nelements != 6)
    			throw (MotionFileException(MotionFileException::formaterror, "missing data for forceplate corners"));
        
				double corner_vector[3] = { 0, 0, 0 };
				for (size_t j = 0; j < 3; j++)
					corner_vector[j] = 0.1 * ((const Int16*)&data[VAR_FORCE_PLATE_POSITION][4*iplate+i][0])[j];

				Outline[i].SetVector(corner_vector);
      }
    }

    // Kistler correction factors
		size_t coeff_num_rows = data[VAR_FORCE_PLATE_COP_COEFFS].size();
    if (coeff_num_rows == 2)
    {
			// build CoP correction matrix if possible
			double kistler_params[2][6];
			bool have_CoP_correction = true;
      for (size_t r = 0; r < 2; r++)
			{
				size_t coeff_num_cols = data[VAR_FORCE_PLATE_COP_COEFFS][r].size()/4;
				if (coeff_num_cols != 6)
				{
					have_CoP_correction = false;
					break;
				}
				for (size_t c = 0; c < 6; c++)
				{
          kistler_params[r][c] = ((float*)&data[VAR_FORCE_PLATE_COP_COEFFS][r][0])[c];
        }
			}

			// put into calibration matrix
			if (have_CoP_correction)
			{
				for (size_t r = 0; r < 2; r++)
				{
					for (size_t c = 0; c < 6; c++)
					{
						this->COPOptimisation.Add(kistler_params[r][c]);
					}
				}
			}
    }

		// In earlier versions of Codamotion Analysis, sensor offset for AMTI plates was not specified
		// This code will estimate sensor offsets from the data
    if (HasAMTIChannelScheme()  /* &&
				fabs(sensor.x[0]) < 0.1 &&
				fabs(sensor.x[1]) < 0.1 */ )
    {
			// number of frames of data
      size_t nframes_AMTI = data[VAR_ANALOGUE_FORCE][8*iplate+2].size() / 2;

			// matrices for best-fit solution for centre-of-pressure value
			float* linsolveA = new float [(2*nframes_AMTI)*3];
			float* linsolveb = new float [(2*nframes_AMTI)*1];
			Int32 validframe = 0;

			// alternative matrix for solution to equations from MA6.3
			float* linsolveA_MA63 = new float [(2*nframes_AMTI)*3];

			// fill matrices from data
      for (size_t index_frame = 0; index_frame < nframes_AMTI; index_frame++)
      {
				size_t platechan0 = iplate*8;

        // get scaled inputs
        float Fy = -1.0f *
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0  ][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0  ][0]);
        float Fx = -1.0f *
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0+1][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0+1][0]);
        float Fz = 
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0+2][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0+2][0]);
        float My = 1000.0f *
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0+3][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0+3][0]);
        float Mx = 1000.0f *
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0+4][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0+4][0]);
        
				// Mz would be like this but not needed for these calcs
				// float Mz = 1000.0f *
        //    ((Int16*)&(data[FileFormatMDF::VAR_ANALOGUE_FORCE][platechan0+5][0]))[index_frame]
        //    * *(float*)(&data[FileFormatMDF::VAR_FORCE_RESOLUTION][platechan0+5][0]);

				// centre of pressure as originally computed
				float Py = 
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0+6][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0+6][0]);
        float Px = 
            ((Int16*)&(data[VAR_ANALOGUE_FORCE][platechan0+7][0]))[index_frame]
            * *(float*)(&data[VAR_FORCE_RESOLUTION][platechan0+7][0]);

				if (fabs(Fz) < 1.0)
					continue;

				//-- fill matrices

				// first row of matrix
				linsolveA[6*validframe  ] = Fz;
				linsolveA[6*validframe+1] = 0.0f;    
				linsolveA[6*validframe+2] = -Fx;

				// second row of matrix
				linsolveA[6*validframe+3] = 0.0f;
				linsolveA[6*validframe+4] = Fz;    
				linsolveA[6*validframe+5] = -Fy;

				// first row of const
				linsolveb[2*validframe  ] = Px*Fz + My;

				// second row of const
				linsolveb[2*validframe+1] = Py*Fz - Mx;

				//-- fill alternative matrices (works for files saved with MA6.3)

				// first row of matrix (note different sign of Fx component)
				linsolveA_MA63[6*validframe  ] = Fz;
				linsolveA_MA63[6*validframe+1] = 0.0f;    
				linsolveA_MA63[6*validframe+2] = Fx;

				// second row of matrix (same as before)
				linsolveA_MA63[6*validframe+3] = 0.0f;
				linsolveA_MA63[6*validframe+4] = Fz;    
				linsolveA_MA63[6*validframe+5] = -Fy;

				// another valid frame
				validframe++;
			}

			if (validframe > 2)
			{
				// solve to determine sensor consts from data
				float x[3], x_MA63[3];
				float rms(0.0f), rms_MA63(0.0f);
				if (LinearSolve3(x, linsolveA, linsolveb, validframe*2, &rms))
				{
					// also solve for MA63 equations
					// use them only if: 
					// - they solve ok
					// - they produce a better rms result than the regular equations
					// - they give a positive value for z0 as was used in those equations
					if (LinearSolve3(x_MA63, linsolveA_MA63, linsolveb, validframe*2, &rms_MA63) &&
						  (rms_MA63 < rms) &&
							(x_MA63[2] > 0.0))
					{
						CentreOffset.X = RoundSensorConst(x_MA63[0]);
						CentreOffset.Y = RoundSensorConst(x_MA63[1]);
						CentreOffset.Z = RoundSensorConst(x_MA63[2]);
					}
					else
					{
						CentreOffset.X = RoundSensorConst(x[0]);
						CentreOffset.Y = RoundSensorConst(x[1]);
						CentreOffset.Z = RoundSensorConst(fabs(x[2]));
					}
				}
			}

			// done with matrices
			delete [] linsolveA;
			delete [] linsolveb;
			delete [] linsolveA_MA63;
    }

		// need to know number of EMG channels because force data always begins after it
		Int32 num_emg = (Int32)data[VAR_ANALOGUE_EMG].size();

		// find one-based indices into channels
		bool AMTIscheme = HasAMTIChannelScheme();
		for (Int32 ichannel = 0; ichannel < 8; ichannel++)
		{
			Int32 analogchannel = num_emg + 8*iplate + ichannel + 1;
			if (AMTIscheme && ichannel >= 6)
				this->PreCompCoPChannels.Add(analogchannel);
			else
				this->Channels.Add(analogchannel);
		}
	}

	// round k to nearest 0.1
	double ForcePlateMDF::RoundSensorConst(double k)
	{
		return 0.1*floor(10.0*k + 0.5);
	}

	UInt8 ForcePlateMDF::MDFPlateID() const throw(MotionFileException)
  {
		UInt8 id(0);
    const std::string& model = Model.Value();
		if (Type.Value() == ForcePlate::TypeKistler)
    {
      if (model == "9821B")
        id = type_Kistler_9821B;
      else if (model == "9826")
        id = type_Kistler_9826;
      else if (model == "9821C_1")
        id = type_Kistler_9821C_1;
      else if (model == "9821C_2")
        id = type_Kistler_9821C_2;
      else if (model == "9827")
        id = type_Kistler_9827;
      else if (model == "9261")
        id = type_Kistler_9261;
      else
        id = type_Kistler_9821B;

    }
		else if (Type.Value() == ForcePlate::TypeAMTI)
    {
      if (model == "BP2416")
        id = type_AMTI_BP2416;
      else if (model == "LG6")
        id = type_AMTI_LG6;
			else if (model == "OR6")
        id=type_AMTI_OR6_7;
      else if (model == "AccuSway")
        id = type_AMTI_AccuSway;
      else if (model == "AccuGait")
				id=type_AMTI_AccuGait;
			else
        id = type_AMTI_BP2416;
    }
		else
		{
			throw (MotionFileException(MotionFileException::notsupported, "unsupported force plate type"));
		}

		return id;
  }

  const char* ForcePlateMDF::IDtoType(UInt8 id) throw(MotionFileException)
  {
    switch((int)id)
    {
    case type_Kistler_9821B:
    case type_Kistler_9826:
    case type_Kistler_9821C_1:
    case type_Kistler_9821C_2:
    case type_Kistler_9827:
    case type_Kistler_9261:
			return ForcePlate::TypeKistler;

    case type_AMTI_BP2416:
    case type_AMTI_LG6:
		case type_AMTI_OR6_7:
    case type_AMTI_AccuSway:
    case type_AMTI_AccuGait:
			return ForcePlate::TypeAMTI;

    default:
			throw (MotionFileException(MotionFileException::formaterror, "unrecognised force plate ID"));
      return "";
    }
  }

  const char* ForcePlateMDF::IDtoModel(UInt8 id) throw(MotionFileException)
  {
    switch((int)id)
    {
    case type_Kistler_9821B:
      return "9821B";

    case type_Kistler_9826:
      return "9826";

    case type_Kistler_9821C_1:
      return "9821C_1";

    case type_Kistler_9821C_2:
      return "9821C_2";

    case type_Kistler_9827:
      return "9827";

    case type_Kistler_9261:
      return "9261";

    case type_AMTI_BP2416:
      return "BP2416";

    case type_AMTI_LG6:
      return "LG6";

		case type_AMTI_OR6_7:
			return "OR6";

    case type_AMTI_AccuSway:
      return "AccuSway";

		case type_AMTI_AccuGait:
      return "AccuGait";

    default:
			throw (MotionFileException(MotionFileException::formaterror, "unrecognised force plate ID"));
      return "";
    }
  }

	bool ForcePlateMDF::HasAMTIChannelScheme() const
	{
		bool isAMTI = (Type.Value() == ForcePlate::TypeAMTI);
		return isAMTI;
	}

	void ForcePlateMDF::DefaultSensorConstants(UInt8 id)
  {
    switch((int)id)
    {
    case type_Kistler_9821B:
      // sensor_separation = Vector3(120.0, 200.0, 54.0);
			SensorSeparation.X = 120.0;
			SensorSeparation.Y = 200.0;
			SensorSeparation.Z = 54.0; 
      break;

    case type_Kistler_9826:
      // sensor_separation = Vector3(120.0F, 200.0F, 49.0F);
			SensorSeparation.X = 120.0;
			SensorSeparation.Y = 200.0;
			SensorSeparation.Z = 49.0; 
      break;

    case type_Kistler_9821C_1:
      // sensor_separation = Vector3(120.0F, 200.0F, 49.0F);
			SensorSeparation.X = 120.0;
			SensorSeparation.Y = 200.0;
			SensorSeparation.Z = 49.0; 
      break;

    case type_Kistler_9821C_2:
      // sensor_separation = Vector3(175.0F, 275.0F, 22.0F);
			SensorSeparation.X = 175.0;
			SensorSeparation.Y = 275.0;
			SensorSeparation.Z = 22.0; 
      break;

    case type_Kistler_9827:
      // sensor_separation = Vector3(210.0F, 350.0F, 52.0F);
			SensorSeparation.X = 210.0;
			SensorSeparation.Y = 350.0;
			SensorSeparation.Z = 52.0; 
      break;

    case type_Kistler_9261:
      // sensor_separation = Vector3(132.0F, 220.0F, 37.0F);
			SensorSeparation.X = 132.0;
			SensorSeparation.Y = 220.0;
			SensorSeparation.Z = 37.0; 
      break;

    case type_AMTI_BP2416:
    case type_AMTI_LG6:
		case type_AMTI_OR6_7:
    case type_AMTI_AccuSway:
      // centre_offset = Vector3(0.0F, 0.0F, 61.0F);
			CentreOffset.X = 0.0;
			CentreOffset.Y = 0.0;
			CentreOffset.Z = 61.0;
      break;

		case type_AMTI_AccuGait:
      // centre_offset = Vector3(0.0F, 0.0F, 61.0F);
			CentreOffset.X = 0.0;
			CentreOffset.Y = 0.0;
			CentreOffset.Z = 61.0;
      break;
    }
  }

  void ForcePlateMDF::DefaultOutline()
  {
    Outline[0].X = -300.0;
    Outline[0].Y = -200.0;
    Outline[0].Z = 0.0;
    Outline[1].X = -300.0;
    Outline[1].Y = 200.0;
    Outline[1].Z = 0.0;
    Outline[2].X = 300.0;
    Outline[2].Y = 200.0;
    Outline[2].Z = 0.0;
    Outline[3].X = 300.0;
    Outline[3].Y = -200.0;
    Outline[3].Z = 0.0;
  }

  float ForcePlateMDF::MDFDefaultSaveRes(size_t ichannel) const
  {
    const float resAMTI[8] = { 0.01f, 0.01f, 0.06f, 0.03f, 0.03f, 0.01f, 0.1f, 0.1f };
    const float resKistler[8] = { 0.02f, 0.02f, 0.02f, 0.02f, 0.06f, 0.06f, 0.06f, 0.06f };

    if (ichannel > 7)
      return 1.0;

    if (HasAMTIChannelScheme())
      return resAMTI[ichannel];
    else
      return resKistler[ichannel];
  }

  const char* ForcePlateMDF::MDFChannelUnits(size_t mdfchannel) const
  {
    if (HasAMTIChannelScheme())	
		{
			switch (mdfchannel)
			{
				// Nmm for moments channels
			case 3:
			case 4:
			case 5:
				return "Nmm";

				// mm for pre-computed CoP values
			case 6:
			case 7:
				return "mm";
			}
		}

		// default for all others (and all channels on Kistlers) is Newtons
		return "N";
  }

	//--------------------------------------------------------------------------
	// channel mapping for psuedo-rotation of force platform

	// - index into the array using index for MDF writing
	// - get as a result the index of input channel (runtime channel) to use
	// - one map for each orientation
	// - minus signs mean to negate the input data

	// Kistler
	static const int mdf_remap_kistler[4][8] = 
	{
		{  1,  2,  3,  4,  5,  6,  7,  8 },
		{ -3, -4,  2,  1,  8,  5,  6,  7 },
		{ -2, -1, -4, -3,  7,  8,  5,  6 },
		{  4,  3, -1, -2,  6,  7,  8,  5 },
	};

	// AMTI
	static const int mdf_remap_amti[4][6] = 
	{
		{ -2, -1,  3,  5,  4,  6 },
		{ -1,  2,  3,  4, -5,  6 },
		{  2,  1,  3, -5, -4,  6 },
		{  1, -2,  3, -4,  5,  6 }
	};

	//--------------------------------------------------------------------------

	size_t ForcePlateMDF::MDFChannelToRuntimeChannel(size_t mdfchannel) const
	{
		// get orientation
		Orientation orient = MDFOrientation();

		// cannot remap unless orientation 0-3
		if (orient < 0 || orient >= 4)
			return mdfchannel;

		// remap according to plate type
		int mapvalue = 0;
		if (HasAMTIChannelScheme())
		{
			if (mdfchannel < 6)
				mapvalue = mdf_remap_amti[orient][mdfchannel];
		}
		else
		{
			if (mdfchannel < 8)
				mapvalue = mdf_remap_kistler[orient][mdfchannel];
		}

		// extract zero-based integer part of map value
		if (mapvalue)
		{
			int runtimechannel = abs(mapvalue) - 1;
			return runtimechannel;
		}

		// no remapping available or needed - return original value
		return mdfchannel;
	}

	size_t ForcePlateMDF::RuntimeChannelToMDFChannel(size_t runchannel) const
	{
		// get orientation
		Orientation orient = MDFOrientation();

		// cannot remap unless orientation 0-3
		if (orient < 0 || orient >= 4)
			return runchannel;

		// remap according to plate type
		if (HasAMTIChannelScheme())
		{
			for (int i = 0; i < 6; i++)
			{
				if ((abs(mdf_remap_amti[orient][i])-1) == static_cast<int>(runchannel))
					return i;
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if ((abs(mdf_remap_kistler[orient][i])-1) == static_cast<int>(runchannel))
					return i;
			}
		}

		// no remapping available or needed - return original value
		return runchannel;
	}

  double ForcePlateMDF::MDFChannelScale(size_t mdfchannel) const
  {
		// get orientation
		Orientation orient = MDFOrientation();

		// cannot remap unless orientation 0-3
		if (orient < 0 || orient >= 4)
			return 1.0;

		if (HasAMTIChannelScheme())
		{
			// scale conversion of Nm -> Nmm for moments components of AMTI data
			// and change sign of Fx, Fy according to required remapping
			const static double AMTImagnitude[6] = { 1.0, 1.0, 1.0, 1000.0, 1000.0, 1000.0 };
			if (mdfchannel < 6)
			{
				double scale = (mdf_remap_amti[orient][mdfchannel] > 0) ? AMTImagnitude[mdfchannel] : -AMTImagnitude[mdfchannel];
				return scale;
			}
		}
		else
		{
			// 1 or -1 for Kistler depending on how specified in the remapping
			if (mdfchannel < 8)
			{
				double scale = (mdf_remap_kistler[orient][mdfchannel] > 0) ? 1.0 : -1.0;
				return scale;
			}
		}

		// usual scale is 1
		return 1.0;
  }

  void ForcePlateMDF::ParseMDFSensorConstants(const Int16* w)
  {
		// swap x & y and force correct sign of AMTI Z offset
		if (HasAMTIChannelScheme())
		{
			// scale appropriately (strain gauge)
			CentreOffset.X = 0.1*w[0];
			CentreOffset.Y = 0.1*w[1];
			CentreOffset.Z = 0.1*w[2];

			if (CentreOffset.Z > 0.0)
				CentreOffset.Z.Value() *= -1.0;
			
			// swap sensor_const[0] and sensor_const[1]
			// problem in VS2010 using std::swap
			// std::swap<double>(sensor_const[0], sensor_const[1]);
			double temp = CentreOffset.X;
			CentreOffset.X = CentreOffset.Y;
			CentreOffset.Y = temp;
		}
		else
		{
			// scale appropriately (kistler)
			SensorSeparation.X = 0.1*w[0];
			SensorSeparation.Y = 0.1*w[1];
			SensorSeparation.Z = 0.1*w[2];
		}
  }

  void ForcePlateMDF::MDFSensorConstants(Int16* w) const
  {
		size_t index_x(0), index_y(1);
		Int16 scale_x(1), scale_y(1);
		double sensor_const[3];

		if (HasAMTIChannelScheme())
		{
			// Must remap according to convention for AMTI
			index_x = RuntimeChannelToMDFChannel(0);
			index_y = RuntimeChannelToMDFChannel(1);
			scale_x = (MDFChannelScale(0) >= 0.0) ? -1 : 1;		// opposite of channel scale
			scale_y = (MDFChannelScale(1) >= 0.0) ? -1 : 1;   // opposite of channel scale
			CentreOffset.GetVector(sensor_const);
		}
		else
		{
			// For Kistler these are dimensions so need to swap X and Y if at 90 or 270
			Orientation orient = MDFOrientation();
			if (orient == orient090 || orient == orient270)
			{
				index_x = 1;
				index_y = 0;
			}
			SensorSeparation.GetVector(sensor_const);
		}

    // get consts appropriately and apply standard MDF scale of 10
    w[0] = scale_x * (Int16)(10.0*sensor_const[index_x]);
    w[1] = scale_y * (Int16)(10.0*sensor_const[index_y]);
    w[2] = (Int16)(10.0*sensor_const[2]);
	}

	// deduce MDF orientation from Outline (may be invalid)
	ForcePlateMDF::Orientation ForcePlateMDF::MDFOrientation() const
	{
		RigidTransform3 T;
		Vector3 outline_vector[4];
		for (size_t corner_index = 0; corner_index < 4; corner_index++)
			Outline[corner_index].GetVector(outline_vector[corner_index]);

		// mean central point
    Vector3 centre(0,0,0);
    centre += outline_vector[0];
    centre += outline_vector[1];
    centre += outline_vector[2];
    centre += outline_vector[3];
    centre *= 0.25;

    // mean fp X-direction
    Vector3 meanX(0,0,0);
    meanX -= outline_vector[0];
    meanX += outline_vector[1];
    meanX += outline_vector[2];
    meanX -= outline_vector[3];

    // mean fp Y-direction
    Vector3 meanY(0,0,0);
    meanY -= outline_vector[0];
    meanY -= outline_vector[1];
    meanY += outline_vector[2];
    meanY += outline_vector[3];

    // transform fp co-ords ---> global co-ords
    RigidTransform3::FromXYVec(T, centre, meanX, meanY);

		// this means plate must lie horizontal to within 0.1 degrees
		// - usually Codamotion files specify the plate as exactly horizontal but
		//   some manufacturers do not impose such a constraint
		double zcomponent = T.R(2,2);
		if (fabs(zcomponent - -1.0) > 0.0017)
		{
			return orient_invalid;
		}

		// get rotation angle - atan2 returns a range [-pi, pi) 
		// or is it (-pi, pi] I'm not sure but it doesn't affect the outcome
		const double PI = 3.1415926535897932384626433832795;
		double angle = 180.0 * atan2( T.R(0,1), T.R(0,0) ) / PI;

		// convert to range [0, 360)
		if (angle < 0.0)
			angle += 360.0;

		// find value 
		// - note how the MDF (Codamotion Analysis) definition of orientation is 
		//   90 deg out of phase with the actual matrix orientation & opposite sense
		if (angle > 359.5 || angle < 0.5)
			return orient090;
		else if (fabs(angle-90.0) < 0.5)
			return orient000;
		else if (fabs(angle-180.0) < 0.5)
			return orient270;
		else if (fabs(angle-270.0) < 0.5)
			return orient180;
		else
			return orient_invalid;
	}

	// make MDF-compatible outline to match ChannelScale and RuntimeChanneltoMDFChannel
	// returns true if made ok, false if invalid orientation
	bool ForcePlateMDF::MDFOutline(std::vector<Vector3>& mdfoutline) const
	{
		// get orientation
		Orientation orient = MDFOrientation();
		
		// must be valid
		if (orient == orient_invalid)
			return false;

		// set result by rotating to canonical system
		mdfoutline.resize(4);
		for (size_t corner_index = 0; corner_index < 4; corner_index++)
		{
			Outline[(corner_index + (size_t)orient) % 4].GetVector(mdfoutline[corner_index]);
		}

		// set ok
		return true;
	}
}
