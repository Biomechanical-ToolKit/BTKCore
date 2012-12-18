/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _MDF_VAR_TYPES_H_
#define _MDF_VAR_TYPES_H_

namespace Open3DMotion
{
	enum MDF_VAR_TYPE
  {
    VAR_TEXT_COMMENT,           // User comments.  Only one comment is allowed per file
    VAR_DATE_CREATE,            // Date of creation.  Only one per file.
    VAR_POSITION_MARKER,        // Marker position
    VAR_ANALOGUE_FORCE,         // Force data
    VAR_ANALOGUE_EMG,           // EMG data
    VAR_FLAG_IN_VIEW,           // Each bit corresponds to a marker epoch and is TRUE if marker is in view
    VAR_FLAG_EVENT,             // Logic data
    VAR_TIME_RESOLUTION_MARKER, // Time resolution of one marker epoch (in 1 / 100000 seconds)
    VAR_TIME_RESOLUTION_FORCE,  
    VAR_TIME_RESOLUTION_EMG,
    VAR_TIME_RESOLUTION_EVENT,
    VAR_TIME_RESOLUTION_CURSOR,
    VAR_TIME_CURSOR,
    VAR_POSITION_RESOLUTION_MARKER, // Position resolution of one interger of marker position (in 1/1000 mm)
    VAR_MARKER_NUMBER_HARDWARE,     // The hardware marker identifiers
    VAR_TIME_ZOOM,
    VAR_TIME_RESOLUTION_ZOOM,
    VAR_GAIT_CYCLES,
    VAR_FORCE_RESOLUTION,
    VAR_EMG_RESOLUTION, 
    VAR_FORCE_PLATE_CONSTANTS,
    VAR_FORCE_PLATE_POSITION,
    VAR_MARKER_NAMES,
    VAR_ANALOGUE_FORCE_NAMES,
    VAR_ANALOGUE_EMG_NAMES,
    VAR_PATIENT_ID,
    VAR_PATIENT_CLASSIFICATION,
    VAR_PATIENT_DATA,
    VAR_PATIENT_SEGMENT_DATA,
    VAR_PATIENT_DATA_NAMES,
    VAR_VIDEO_AVI,		           // VIDEO
    VAR_FORCE_PLATE_TYPE,
    VAR_FORCE_PLATE_FLAGS,
    VAR_FORCE_PLATE_COP_COEFFS,
    VAR_ADCEMG_CHANNELNUM,
    VAR_MARKER_INTENSITY,
		VAR_FORCE_SEGMENT,

    // calc data descriptors
    DataChannelNames = 50,
    DataGroupName,
    DataScaling,

    // calc data types
    DataMarker = 53,            // 1   3D
    DataForce,             // 2   3D
    DataForceVector,       // 3   3D
    DataEMG,               // 4
    DataVelocity,          // 5   3D
    DataAcceleration,      // 6   3D
    DataJointAngle,        // 7
    DataJAngVelocity,      // 8
    DataMoment,            // 9
    DataPower,             // 10
    DataLength,            // 11
    DataVectorAngle,       // 12
    DataVAngVelocity,      // 13
    DataVAngAcceleration,  // 14
    DataJointMoment,       // 15
    DataJointPower,        // 16       //!!last datatype common with Version 2.x
    DataSegRefPoint,       // 17  3D
    DataSegRotation,       // 18  3D
    DataSegJCVel,          // 19  3D
    DataSegJCAcc,          // 20  3D
    DataSegEVBx,           // 21  3D
    DataSegEVBy,           // 22  3D
    DataSegEVBz,           // 23  3D
    DataSegAngVel,         // 24  3D
    DataSegAngAcc,         // 25  3D
    DataSegJRotVel,        // 26  3D
    DataSegJMoment,        // 27  3D
    DataSegJPowerSum,      // 28
    DataSegJPower3D,       // 29  3D
    DataAnalogue,          // 30      //!!this is out of synch with Version 2.x
    DataDigital,           // 31
    DataVirtualMarker,     // 32
    DataVMVelocity,        // 33
    DataVMAcceleration,    // 34
    DataRigidBodyPosn,     // 35
    DataRigidBodyVely,     // 36
    DataRigidBodyRotn,     // 37
    DataRigidBodyEVBx,     // 38
    DataRigidBodyEVBy,     // 39
    DataRigidBodyEVBz,     // 40
    DataMarkerSpeed,       // 41
    DataMarkerAccMag,      // 42
    DataMarkerInView,      // 43  3D (ABC)
    NumDataTypes           // 44
  };

}

#endif
