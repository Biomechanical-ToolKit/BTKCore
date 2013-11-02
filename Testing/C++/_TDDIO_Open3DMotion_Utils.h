#ifndef _TDDIO_Open3DMotion_Utils_h
#define _TDDIO_Open3DMotion_Utils_h

#include "_TDDIO_Open3DMotion_Ressources.h"

#include <btkAcquisitionFileReader.h>

void btk_o3dm_ADemo1_test(const std::string& filename)
{
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetFilename(filename);
  reader->Update();
  
  btk::Acquisition::Pointer acq = reader->GetOutput();
  
  TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
  TS_ASSERT_EQUALS(acq->GetPointNumber(), (int)o3dm_ADemo1_nummarkers);
  TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), (int)o3dm_ADemo1_numframes_marker);
  TS_ASSERT_EQUALS(acq->GetPointFrequency(), o3dm_ADemo1_rateHz_marker);
  TS_ASSERT_EQUALS(acq->GetAnalogNumber(), (int)o3dm_ADemo1_numEMG + 8);
  TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), (int)o3dm_ADemo1_numframes_marker * 2);
  TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), o3dm_ADemo1_rateHz_EMG);
  TS_ASSERT_EQUALS(acq->GetNumberAnalogSamplePerFrame(), 2);
  
  for (int i = 0 ; i < (int)o3dm_ADemo1_nummarkers ; ++i)
    TS_ASSERT_EQUALS(acq->GetPoint(i)->GetLabel(), o3dm_ADemo1_markername[i]);
  
  for (int i = 0 ; i < (int)o3dm_ADemo1_nummarkers ; ++i)
  {
    for (int j = 0 ; j < (int)o3dm_ADemo1_numframes_marker ; ++j)
    {
      int inc = (j * o3dm_ADemo1_nummarkers + i) * 4;
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues().coeff(j,0), o3dm_ADemo1_marker[inc], 1e-1);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues().coeff(j,1), o3dm_ADemo1_marker[inc + 1], 1e-1);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues().coeff(j,2), o3dm_ADemo1_marker[inc + 2], 1e-1);
      TS_ASSERT_EQUALS(acq->GetPoint(i)->GetResiduals().coeff(j) < 0.0, o3dm_ADemo1_marker[inc + 3] < 1e-12);
    }
  }
  
  for (int i = 0 ; i < (int)o3dm_ADemo1_numEMG ; ++i)
    TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), o3dm_ADemo1_EMGname[i]);
  
  for (int i = 0 ; i < (int)o3dm_ADemo1_numEMG ; ++i)
  {
    btk::Analog::Pointer an = acq->GetAnalog(i);
    for (int j = 0 ; j < (int)o3dm_ADemo1_numframes_EMG ; ++j)
    {
      TS_ASSERT_DELTA(an->GetValues().coeff(j,0), o3dm_ADemo1_EMG[j * o3dm_ADemo1_numEMG + i], 1e-4);
    }
  }
};

#endif // _TDDIO_Open3DMotion_Utils_h