#ifndef C3DFileReaderTest_h
#define C3DFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkC3DFileIO.h>

CXXTEST_SUITE(C3DFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.c3d");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.c3d"));
  };
  
  CXXTEST_TEST(Empty)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/Empty.c3d");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("No IO found, the file is not supported or valid or the file suffix is misspelled (Some IO use it to verify they can read the file)\nFilename: " + C3DFilePathIN + "others/Empty.c3d"));
  };
  
  CXXTEST_TEST(Sample01_Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    
  };
  
  CXXTEST_TEST(Sample01_Eb015si)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015si.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015vi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vi.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015sr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015sr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1), -25.8, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
  };
  
  CXXTEST_TEST(Sample02_DEC_INT)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample02/DEC_INT.C3D");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);

    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RSK1");
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8418, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 0.0001);

    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "FZ1");
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 0.0001);
  };

  CXXTEST_TEST(Sample02_Dec_real)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample02/Dec_real.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);

    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RSK1");
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8417, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 0.0001);

    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "FZ1");
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 0.0001);
  };

  CXXTEST_TEST(Sample02_pc_int)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample02/pc_int.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);

    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RSK1");
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.5606, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 0.0001);

    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "FZ1");
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 0.0001);
  };

  CXXTEST_TEST(Sample02_pc_real)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample02/pc_real.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);

    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RSK1");
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8417, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 0.0001);

    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "FZ1");
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 0.0001);
  };

  CXXTEST_TEST(Sample02_sgi_int)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample02/sgi_int.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);

    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RSK1");
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.5606, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 0.0001);

    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "FZ1");
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 0.0001);
  };

  CXXTEST_TEST(Sample02_sgi_real)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample02/sgi_real.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);

    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RSK1");
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8417, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 0.0001);

    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "FZ1");
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 0.0001);
  };

  CXXTEST_TEST(Sample05_vicon512)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample05/vicon512.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 120);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 45);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 6492);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1080);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 23);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 6492 * 9);
  };

  CXXTEST_TEST(Sample06_MACsample)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample06/MACsample.c3d");
    reader->Update();
  };


  CXXTEST_TEST(Sample08_TEST)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    // TESTAPI
    reader->SetFilename(C3DFilePathIN + "sample08/TESTAPI.c3d");
    reader->Update();
    btk::Acquisition::Pointer TESTAPI = reader->GetOutput();
    // TESTBPI
    reader->SetFilename(C3DFilePathIN + "sample08/TESTBPI.c3d");
    reader->Update();
    btk::Acquisition::Pointer TESTBPI = reader->GetOutput();
    // TESTCPI
    reader->SetFilename(C3DFilePathIN + "sample08/TESTCPI.c3d");
    reader->Update();
    btk::Acquisition::Pointer TESTCPI = reader->GetOutput();
    // TESTDPI
    reader->SetFilename(C3DFilePathIN + "sample08/TESTDPI.c3d");
    reader->Update();
    btk::Acquisition::Pointer TESTDPI = reader->GetOutput();
  };

  CXXTEST_TEST(Sample09_PlugInC3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetType(), btk::Point::Marker);
    TS_ASSERT_EQUALS(acq->GetPoint(18)->GetType(), btk::Point::Angle);
    TS_ASSERT_EQUALS(acq->GetPoint(64)->GetType(), btk::Point::Force);
    TS_ASSERT_EQUALS(acq->GetPoint(72)->GetType(), btk::Point::Moment);
    TS_ASSERT_EQUALS(acq->GetPoint(80)->GetType(), btk::Point::Power);
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(8)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(9)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(10)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(11)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(12)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(13)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(16)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(17)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(18)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(19)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(20)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(21)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(22)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(23)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(24)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(25)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(26)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(27)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(28)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(29)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(30)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(31)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(32)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(33)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(34)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq->GetAnalog(35)->GetGain(), btk::Analog::PlusMinus5);
    
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq->GetMetaData()->GetChild("POINT")->GetChild("FRAMES")->GetInfo()->ToUInt16(0));
    btk::MetaDataInfo::Pointer actualFieldVal = acq->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo();
    int frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), frameIndex);
    actualFieldVal = acq->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo();
    frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
    TS_ASSERT_EQUALS(acq->GetLastFrame(), frameIndex);
  }

  CXXTEST_TEST(Sample13_Dance)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample13/Dance.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_DELTA(acq->GetPointFrequency(), 65.05, 0.01);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 40);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 499);
    TS_ASSERT_DELTA(acq->GetAnalogFrequency(), 65.05, 0.01);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 499);
  };

  CXXTEST_TEST(Sample13_golfswing)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample13/golfswing.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_DELTA(acq->GetPointFrequency(), 107.53, 0.01);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 29);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 514);
    TS_ASSERT_DELTA(acq->GetAnalogFrequency(), 107.53, 0.01);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 514);
  };

  CXXTEST_TEST(Sample15_FP1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample15/FP1.C3D");
    reader->Update();
  };
  
  CXXTEST_TEST(Sample15_FP2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample15/FP2.C3D");
    reader->Update();
  };

  CXXTEST_TEST(Sample18_bad_parameter_section)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample18/bad_parameter_section.c3d");

    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::C3DFileIOException &e, e.what(), std::string("Bad data first block"));
  };

  CXXTEST_TEST(Sample19_sample19)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample19/sample19.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 60);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 34672);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1080);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 2);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 18 * 34672);
  };

  CXXTEST_TEST(Sample20_phasespace_sample)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample20/phasespace_sample.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 40);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 701);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 701);
  };

  CXXTEST_TEST(Sample21_sample21)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample21/sample21.c3d");
    reader->Update();
  };

  CXXTEST_TEST(Sample22_BKINtechnologies)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample22/BKINtechnologies.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 2);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "STAY_CENTRE");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 0.0, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetDescription(), "subject must wait at centre starting now");
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "TARGET_ON");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 1.6140, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetDescription(), "Target light ON, start waiting for subject to leave centre target");
  };

  CXXTEST_TEST(Sample28_dynamic)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample28/dynamic.C3D");
    reader->Update();
  };

  CXXTEST_TEST(Sample28_standing)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample28/standing.C3D");
    reader->Update();
  };

  CXXTEST_TEST(Sample28_type1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample28/type1.C3D");
    reader->Update();
  };
  
  CXXTEST_TEST(ParameterOverflow)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/parameterOverflow.C3D");
    reader->Update();
    
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::MetaData::ConstIterator itAnalysis = acq->GetMetaData()->FindChild("ANALYSIS");
    TS_ASSERT(itAnalysis != acq->GetMetaData()->End());
    if (itAnalysis != acq->GetMetaData()->End())
      TS_ASSERT((*itAnalysis)->FindChild("VALUES") != (*itAnalysis)->End());
  };
  
  CXXTEST_TEST(Mocap36)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/mocap36.c3d");
    reader->Update();
  };
};

CXXTEST_SUITE_REGISTRATION(C3DFileReaderTest)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Empty)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015pi)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015si)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015vi)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015sr)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample02_DEC_INT)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample02_Dec_real)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample02_pc_int)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample02_pc_real)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample02_sgi_int)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample02_sgi_real)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample05_vicon512)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample06_MACsample)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample08_TEST)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample09_PlugInC3D)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample13_Dance)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample13_golfswing)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample15_FP1)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample15_FP2)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample18_bad_parameter_section)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample19_sample19)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample20_phasespace_sample)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample21_sample21)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample22_BKINtechnologies)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample28_dynamic)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample28_standing)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample28_type1)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, ParameterOverflow)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Mocap36)
#endif
