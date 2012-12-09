// All the data come from the file 'src/Open3DMotionTest/Data/MDF/ADemo1.h' distributed within the project Open3DMotion

#ifndef _TDDIO_Open3DMotion_Ressources_h
#define _TDDIO_Open3DMotion_Ressources_h

static const size_t o3dm_ADemo1_nummarkers = 24;
static const size_t o3dm_ADemo1_numforceplates = 1;
static const size_t o3dm_ADemo1_numEMG = 8;
static const double o3dm_ADemo1_rateHz_marker = 200.0;
static const double o3dm_ADemo1_rateHz_force = 200.0;
static const double o3dm_ADemo1_rateHz_EMG = 400.0;
static const size_t o3dm_ADemo1_numframes_marker = 512;
static const size_t o3dm_ADemo1_numframes_force = 511;
static const size_t o3dm_ADemo1_numframes_EMG = 1023;
static const double o3dm_ADemo1_forcetol = 0.05;
extern const double o3dm_ADemo1_marker[o3dm_ADemo1_numframes_marker*o3dm_ADemo1_nummarkers*4];
extern const double o3dm_ADemo1_force[o3dm_ADemo1_numframes_force*o3dm_ADemo1_numforceplates*8];
extern const double o3dm_ADemo1_EMG[o3dm_ADemo1_numframes_EMG*o3dm_ADemo1_numEMG];
extern const char* o3dm_ADemo1_markername[o3dm_ADemo1_nummarkers];
extern const char* o3dm_ADemo1_EMGname[o3dm_ADemo1_nummarkers];

#endif // _TDDIO_Open3DMotion_Ressources_h