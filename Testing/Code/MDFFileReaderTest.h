#ifndef MDFFileReaderTest_h
#define MDFFileReaderTest_h

#include "MDFFileReaderTest_data.h"

#include <btkAcquisitionFileReader.h>
#include <btkConvert.h>

CXXTEST_SUITE(MDFFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.MDF");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.MDF"));
  };
  
  CXXTEST_TEST(Gait_bilateral_1997_Kistlerx1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(MDFFilePathIN + "gait-bilateral-1997-Kistlerx1.mdf");
    reader->Update();
    
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    const int num_markers = 24;
    const int num_marker_frames = 512;
    const int num_EMGs = 8;
    const int num_analogs = 16; // 8 EMGs + 1 forceplate (8 channels)
    const int num_analog_frames = 1024;
    
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 200.0);
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), num_markers);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), num_marker_frames);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 200.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), num_analogs);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), num_analog_frames);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 400.0);
    TS_ASSERT_EQUALS(acq->GetNumberAnalogSamplePerFrame(), 2);
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "L.Sac.Wand");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "R.Sac.Wand");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "L.PSIS");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "L.ASIS");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "L.Post.Fem.");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "L.Ant.Fem.");
    TS_ASSERT_EQUALS(acq->GetPoint(6)->GetLabel(), "L.Post.Tib.");
    TS_ASSERT_EQUALS(acq->GetPoint(7)->GetLabel(), "L.Ant.Tib.");
    TS_ASSERT_EQUALS(acq->GetPoint(8)->GetLabel(), "L.Knee");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetLabel(), "L.Ankle");
    TS_ASSERT_EQUALS(acq->GetPoint(10)->GetLabel(), "L.Heel");
    TS_ASSERT_EQUALS(acq->GetPoint(11)->GetLabel(), "L.Toe");
    TS_ASSERT_EQUALS(acq->GetPoint(12)->GetLabel(), "R.PSIS");
    TS_ASSERT_EQUALS(acq->GetPoint(13)->GetLabel(), "R.ASIS");
    TS_ASSERT_EQUALS(acq->GetPoint(14)->GetLabel(), "R.Post.Fem.");
    TS_ASSERT_EQUALS(acq->GetPoint(15)->GetLabel(), "R.Ant.Fem.");
    TS_ASSERT_EQUALS(acq->GetPoint(16)->GetLabel(), "R.Post.Tib.");
    TS_ASSERT_EQUALS(acq->GetPoint(17)->GetLabel(), "R.Ant.Tib.");
    TS_ASSERT_EQUALS(acq->GetPoint(18)->GetLabel(), "R.Knee");
    TS_ASSERT_EQUALS(acq->GetPoint(19)->GetLabel(), "R.Ankle");
    TS_ASSERT_EQUALS(acq->GetPoint(20)->GetLabel(), "R.Heel");
    TS_ASSERT_EQUALS(acq->GetPoint(21)->GetLabel(), "R.Toe");
    TS_ASSERT_EQUALS(acq->GetPoint(22)->GetLabel(), "L.Hip");
    TS_ASSERT_EQUALS(acq->GetPoint(23)->GetLabel(), "R.Hip");
    
    for (int i = 0 ; i < num_markers ; ++i)
    {
      for (int j = 0 ; j < num_marker_frames ; ++j)
      {
        int inc = (j * num_markers + i) * 4;
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues().coeff(j,0), gait_bilateral_1997_Kistlerx1_markers[inc], 1e-1);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues().coeff(j,1), gait_bilateral_1997_Kistlerx1_markers[inc + 1], 1e-1);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues().coeff(j,2), gait_bilateral_1997_Kistlerx1_markers[inc + 2], 1e-1);
        TS_ASSERT_EQUALS(acq->GetPoint(i)->GetResiduals().coeff(j) < 0.0, gait_bilateral_1997_Kistlerx1_markers[inc + 3] < 1e-12);
      }
    }
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetLabel(), "R.Quads");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetLabel(), "R.Med.Hams");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "R.Ant.Tib");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetLabel(), "R.Gastroc");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetLabel(), "L.Quads");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetLabel(), "L.Med.Hams");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetLabel(), "L.Ant.Tib");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetLabel(), "L.Gastroc");
    
    for (int i = 0 ; i < num_EMGs ; ++i)
    {
      btk::Analog::Pointer an = acq->GetAnalog(i);
      for (int j = 0 ; j < 1023 ; ++j) // There is only 1023 in the MDF fiile
      {
        double val = gait_bilateral_1997_Kistlerx1_emgs[j * num_EMGs + i] / an->GetScale() + static_cast<double>(an->GetOffset());
        TS_ASSERT_DELTA(an->GetValues().coeff(j,0), val, 1e-4);
      }
    }
    
    TS_WARN("TODO: Create tests on analog channels used for force platforms.");
    /*
    for (int i = 0 ; i < 8 ; ++i)
    {
      btk::Analog::Pointer an = acq->GetAnalog(i+8);
      for (int j = 0 ; j < 511 ; ++j) // There is only 511 frames for forces
      {
        double val = gait_bilateral_1997_Kistlerx1_forces[j * 8 + i];
        TSM_ASSERT_DELTA("Channel #" + btk::ToString(i+8) + " - Frame #" + btk::ToString(j), an->GetValues().coeff(j*2,0), val, 1e-4);
      }
    }
    */
  };
};

CXXTEST_SUITE_REGISTRATION(MDFFileReaderTest)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, Gait_bilateral_1997_Kistlerx1)
#endif
