#ifndef C3DFileWriterTest_h
#define C3DFileWriterTest_h

#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>
#include <btkC3DFileIO.h>

CXXTEST_SUITE(C3DFileWriterTest)
{
  CXXTEST_TEST(NoFileNoInput)
  {
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFileWithIO)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::C3DFileIO::Pointer io = btk::C3DFileIO::New();
    writer->SetAcquisitionIO(io);
    TS_ASSERT(io->HasWritingFlag(btk::C3DFileIO::ScalesFromDataUpdate));
    TS_ASSERT(io->HasWritingFlag(btk::C3DFileIO::MetaDataFromDataUpdate));    
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(sample01_Eb015pi_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetWritingFlags(btk::C3DFileIO::None);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015pi.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
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

    
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acq2->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acq2->GetAnalog(1)->GetValues()(i), 0.00001);
    }

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pr_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetWritingFlags(btk::C3DFileIO::None);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pr.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015pi.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
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

    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(i), acq->GetPoint(0)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(i), acq->GetPoint(14)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acq2->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acq2->GetAnalog(1)->GetValues()(i), 0.00001);
    }

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015si.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetWritingFlags(btk::C3DFileIO::None);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
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

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015sr_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015sr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetWritingFlags(btk::C3DFileIO::None);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015sr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015sr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
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

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vi_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetWritingFlags(btk::C3DFileIO::None);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vi.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vi.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
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

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetWritingFlags(btk::C3DFileIO::None);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
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

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_clearAnalog)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAnalog.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAnalog.c3d");
    reader->Update();
    acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(writer->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_clearPoint)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearPoint();
    acq->ClearEvent();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearPoint.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearPoint.c3d");
    reader->Update();
    acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(writer->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.1, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_clearAll)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearPoint();
    acq->ClearAnalog();
    acq->ClearEvent();
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAll.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAll.c3d");
    reader->Update();
    acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pi_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vi_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pi_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vi_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_from_Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pi.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pi.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_from_Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015pi.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015pi.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_from_Eb015sr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015sr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015sr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015sr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015sr_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015sr_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015sr_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pr_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalog();
    acq->ClearEvent();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pr_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015pr_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample09_PlugInC3D_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    reader2->Update();

    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    for (int i = 1 ; i < 50 ; ++i)
    {
      for (int j = 0 ; j < acq->GetPointNumber() ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,0), acq2->GetPoint(j)->GetValues()(i,0), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,1), acq2->GetPoint(j)->GetValues()(i,1), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,2), acq2->GetPoint(j)->GetValues()(i,2), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetResiduals()(i), acq2->GetPoint(j)->GetResiduals()(i), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetMasks()(i), acq2->GetPoint(j)->GetMasks()(i), 0.00001);
      }
      for (int j = 0 ; j < acq->GetAnalogNumber() ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(j)->GetValues()(i), acq2->GetAnalog(j)->GetValues()(i), 0.00001);
      }
    }
  }

  CXXTEST_TEST(sample19_sample19_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample19/sample19.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample19_sample19.c3d");
    //writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample19_sample19.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    for (int i = 1 ; i < 50 ; i+=10)
    {
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acq2->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acq2->GetAnalog(1)->GetValues()(i), 0.00001);
    }
  };

  CXXTEST_TEST(convertTRC2C3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "Knee.trc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "Knee_rewritedInC3D.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "Knee_rewritedInC3D.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), -1.0, 0.000001);

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(i,0), acq2->GetPoint(1)->GetValues()(i,0), 0.00005);
      TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(i,1), acq2->GetPoint(2)->GetValues()(i,1), 0.00001);
      TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(i,2), acq2->GetPoint(3)->GetValues()(i,2), 0.00001);
      TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals()(i), acq2->GetPoint(4)->GetResiduals()(i), 0.00001);
    }

    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), acq2->GetPoint(70)->GetResiduals()(82), 0.000001);
  };
};

CXXTEST_SUITE_REGISTRATION(C3DFileWriterTest)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pi_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pr_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015sr_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vi_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_clearAnalog)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_clearPoint)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_clearAll)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pi_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vi_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_from_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pi_from_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vi_from_Eb015pr) 
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_from_Eb015pi) 
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_from_Eb015pi)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_from_Eb015sr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015sr_from_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pr_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample09_PlugInC3D_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample19_sample19_rewrited)  
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, convertTRC2C3D)
#endif
