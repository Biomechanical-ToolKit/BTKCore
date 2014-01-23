#ifndef HPFFileReaderTest_h
#define HPFFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkConvert.h>

CXXTEST_SUITE(HPFFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.hpf");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.hpf"));
  };
  
  CXXTEST_TEST(Run_number_34_VTT_Rep_1DOT6)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(HPFFilePathIN + "Run_number_34_VTT_Rep_1.6.hpf");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 2000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 10752);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 10752);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 2000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 40);
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetLabel(), "R TRAPEZIUS MIDDLE FIBERS: EMG 1");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetLabel(), "R TRAPEZIUS MIDDLE FIBERS: ACC X 1");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "R TRAPEZIUS MIDDLE FIBERS: ACC Y 1");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetLabel(), "R TRAPEZIUS MIDDLE FIBERS: ACC Z 1");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetLabel(), "L TRAPEZIUS MIDDLE FIBERS: EMG 2");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetLabel(), "L TRAPEZIUS MIDDLE FIBERS: ACC X 2");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetLabel(), "L TRAPEZIUS MIDDLE FIBERS: ACC Y 2");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetLabel(), "L TRAPEZIUS MIDDLE FIBERS: ACC Z 2");
    TS_ASSERT_EQUALS(acq->GetAnalog(8)->GetLabel(), "R EXTENSOR DIGITORUM: EMG 3");
    TS_ASSERT_EQUALS(acq->GetAnalog(9)->GetLabel(), "R EXTENSOR DIGITORUM: ACC X 3");
    TS_ASSERT_EQUALS(acq->GetAnalog(10)->GetLabel(), "R EXTENSOR DIGITORUM: ACC Y 3");
    TS_ASSERT_EQUALS(acq->GetAnalog(11)->GetLabel(), "R EXTENSOR DIGITORUM: ACC Z 3");
    TS_ASSERT_EQUALS(acq->GetAnalog(12)->GetLabel(), "L EXTENSOR DIGITORUM: EMG 4");
    TS_ASSERT_EQUALS(acq->GetAnalog(13)->GetLabel(), "L EXTENSOR DIGITORUM: ACC X 4");
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->GetLabel(), "L EXTENSOR DIGITORUM: ACC Y 4");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "L EXTENSOR DIGITORUM: ACC Z 4");
    TS_ASSERT_EQUALS(acq->GetAnalog(16)->GetLabel(), "R FLEXOR CARPI RADIALIS: EMG 5");
    TS_ASSERT_EQUALS(acq->GetAnalog(17)->GetLabel(), "R FLEXOR CARPI RADIALIS: ACC X 5");
    TS_ASSERT_EQUALS(acq->GetAnalog(18)->GetLabel(), "R FLEXOR CARPI RADIALIS: ACC Y 5");
    TS_ASSERT_EQUALS(acq->GetAnalog(19)->GetLabel(), "R FLEXOR CARPI RADIALIS: ACC Z 5");
    TS_ASSERT_EQUALS(acq->GetAnalog(20)->GetLabel(), "L FLEXOR CARPI RADIALIS: EMG 6");
    TS_ASSERT_EQUALS(acq->GetAnalog(21)->GetLabel(), "L FLEXOR CARPI RADIALIS: ACC X 6");
    TS_ASSERT_EQUALS(acq->GetAnalog(22)->GetLabel(), "L FLEXOR CARPI RADIALIS: ACC Y 6");
    TS_ASSERT_EQUALS(acq->GetAnalog(23)->GetLabel(), "L FLEXOR CARPI RADIALIS: ACC Z 6");
    TS_ASSERT_EQUALS(acq->GetAnalog(24)->GetLabel(), "R BICEPS BRACHII: EMG 7");
    TS_ASSERT_EQUALS(acq->GetAnalog(25)->GetLabel(), "R BICEPS BRACHII: ACC X 7");
    TS_ASSERT_EQUALS(acq->GetAnalog(26)->GetLabel(), "R BICEPS BRACHII: ACC Y 7");
    TS_ASSERT_EQUALS(acq->GetAnalog(27)->GetLabel(), "R BICEPS BRACHII: ACC Z 7");
    TS_ASSERT_EQUALS(acq->GetAnalog(28)->GetLabel(), "L BICEPS BRACHII: EMG 8");
    TS_ASSERT_EQUALS(acq->GetAnalog(29)->GetLabel(), "L BICEPS BRACHII: ACC X 8");
    TS_ASSERT_EQUALS(acq->GetAnalog(30)->GetLabel(), "L BICEPS BRACHII: ACC Y 8");
    TS_ASSERT_EQUALS(acq->GetAnalog(31)->GetLabel(), "L BICEPS BRACHII: ACC Z 8");
    TS_ASSERT_EQUALS(acq->GetAnalog(32)->GetLabel(), "R TRICEPS BRACHII: EMG 9");
    TS_ASSERT_EQUALS(acq->GetAnalog(33)->GetLabel(), "R TRICEPS BRACHII: ACC X 9");
    TS_ASSERT_EQUALS(acq->GetAnalog(34)->GetLabel(), "R TRICEPS BRACHII: ACC Y 9");
    TS_ASSERT_EQUALS(acq->GetAnalog(35)->GetLabel(), "R TRICEPS BRACHII: ACC Z 9");
    TS_ASSERT_EQUALS(acq->GetAnalog(36)->GetLabel(), "L TRICEPS BRACHII: EMG 10");
    TS_ASSERT_EQUALS(acq->GetAnalog(37)->GetLabel(), "L TRICEPS BRACHII: ACC X 10");
    TS_ASSERT_EQUALS(acq->GetAnalog(38)->GetLabel(), "L TRICEPS BRACHII: ACC Y 10");
    TS_ASSERT_EQUALS(acq->GetAnalog(39)->GetLabel(), "L TRICEPS BRACHII: ACC Z 10");
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(8)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(9)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(10)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(11)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(12)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(13)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(16)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(17)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(18)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(19)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(20)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(21)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(22)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(23)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(24)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(25)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(26)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(27)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(28)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(29)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(30)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(31)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(32)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(33)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(34)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(35)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(36)->GetUnit(), "V");
    TS_ASSERT_EQUALS(acq->GetAnalog(37)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(38)->GetUnit(), "g");
    TS_ASSERT_EQUALS(acq->GetAnalog(39)->GetUnit(), "g");
    
    for (btk::Acquisition::AnalogConstIterator itA = acq->BeginAnalog() ; itA != acq->EndAnalog() ; ++itA)
    {
      for (int i = 0 ; i < 10 ; ++i)
      {
        TS_ASSERT_DELTA((*itA)->GetValues().coeff(i), 0.0, 1e-5);
      }
    }
    
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(10), 0.3238856, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(10), 1.326369, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(10), -3.640687, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(10), -0.3735166, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(6)->GetValues().coeff(10), -1.078769, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues().coeff(10), -3.737764, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(8)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(9)->GetValues().coeff(10), -2.818379, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(10)->GetValues().coeff(10), 1.680531, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(11)->GetValues().coeff(10), -1.932598, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(12)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(13)->GetValues().coeff(10), -3.235401, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(14)->GetValues().coeff(10), -0.7581546, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(15)->GetValues().coeff(10), -1.25684, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(16)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(17)->GetValues().coeff(10), -2.622296, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(18)->GetValues().coeff(10), 2.348345, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(19)->GetValues().coeff(10), 0.5745529, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(20)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(21)->GetValues().coeff(10), -2.841617, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(22)->GetValues().coeff(10), -1.628069, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(23)->GetValues().coeff(10), 0.3194182, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(24)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(25)->GetValues().coeff(10), -2.612428, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(26)->GetValues().coeff(10), 2.221158, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(27)->GetValues().coeff(10), -1.152982, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(28)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(29)->GetValues().coeff(10), -2.339089, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(30)->GetValues().coeff(10), -2.129879, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(31)->GetValues().coeff(10), -0.7680386, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(32)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(33)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(34)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(35)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(36)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(37)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(38)->GetValues().coeff(10), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(39)->GetValues().coeff(10), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(5135), -4.01E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(5135), -0.06477713, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(5135), 0.5684439, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(5135), -0.8942038, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(5135), -1.51E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(5135), -0.06225277, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(6)->GetValues().coeff(5135), -0.5076562, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues().coeff(5135), -0.9666631, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(8)->GetValues().coeff(5135), 1.09E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(9)->GetValues().coeff(5135), -0.7209805, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(10)->GetValues().coeff(5135), 0.1867257, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(11)->GetValues().coeff(5135), -0.4664891, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(12)->GetValues().coeff(5135), 7.22E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(13)->GetValues().coeff(5135), -0.9059124, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(14)->GetValues().coeff(5135), -0.1895387, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(15)->GetValues().coeff(5135), -0.3142101, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(16)->GetValues().coeff(5135), 5.00E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(17)->GetValues().coeff(5135), -0.6395844, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(18)->GetValues().coeff(5135), 0.6981567, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(19)->GetValues().coeff(5135), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(20)->GetValues().coeff(5135), 6.88E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(21)->GetValues().coeff(5135), -0.7749864, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(22)->GetValues().coeff(5135), -0.6887985, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(23)->GetValues().coeff(5135), 0.1277673, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(24)->GetValues().coeff(5135), 2.23E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(25)->GetValues().coeff(5135), -0.6220067, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(26)->GetValues().coeff(5135), 0.6346164, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(27)->GetValues().coeff(5135), -0.5124366, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(28)->GetValues().coeff(5135), -7.55E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(29)->GetValues().coeff(5135), -0.7386595, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(30)->GetValues().coeff(5135), -0.6454179, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(31)->GetValues().coeff(5135), -0.2560129, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(32)->GetValues().coeff(5135), 1.51E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(33)->GetValues().coeff(5135), -0.6322685, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(34)->GetValues().coeff(5135), -0.1950575, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(35)->GetValues().coeff(5135), 0.5104636, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(36)->GetValues().coeff(5135), 7.17E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(37)->GetValues().coeff(5135), -0.5763358, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(38)->GetValues().coeff(5135), 0.3774208, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(39)->GetValues().coeff(5135), 0.700477   , 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(10751), -2.11E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(10751), -0.06477713, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(10751), 0.6316043, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(10751), -0.8942038, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(10751), 3.19E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(10751), -0.06225277, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(6)->GetValues().coeff(10751), -0.4441991, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues().coeff(10751), -0.9666631, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(8)->GetValues().coeff(10751), 2.52E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(9)->GetValues().coeff(10751), -0.8520679, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(10)->GetValues().coeff(10751), 0.1244838, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(11)->GetValues().coeff(10751), -0.4664891, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(12)->GetValues().coeff(10751), 9.06E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(13)->GetValues().coeff(10751), -0.8412044, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(14)->GetValues().coeff(10751), -0.1895387, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(15)->GetValues().coeff(10751), -0.188526, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(16)->GetValues().coeff(10751), 3.19E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(17)->GetValues().coeff(10751), -0.7035428, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(18)->GetValues().coeff(10751), 0.6981567, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(19)->GetValues().coeff(10751), -0.06383921, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(20)->GetValues().coeff(10751), 6.88E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(21)->GetValues().coeff(10751), -0.7104042, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(22)->GetValues().coeff(10751), -0.5635625, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(23)->GetValues().coeff(10751), 0.2555345, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(24)->GetValues().coeff(10751), 7.89E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(25)->GetValues().coeff(10751), -0.746408, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(26)->GetValues().coeff(10751), 0.5076932, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(27)->GetValues().coeff(10751), -0.5124366, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(28)->GetValues().coeff(10751), -7.22E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(29)->GetValues().coeff(10751), -0.6771045, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(30)->GetValues().coeff(10751), -0.5808761, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(31)->GetValues().coeff(10751), -0.1280064, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(32)->GetValues().coeff(10751), -1.43E-05, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(33)->GetValues().coeff(10751), -0.7587223, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(34)->GetValues().coeff(10751), -0.1950575, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(35)->GetValues().coeff(10751), 0.5104636, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(36)->GetValues().coeff(10751), 1.51E-06, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(37)->GetValues().coeff(10751), -0.5763358, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(38)->GetValues().coeff(10751), 0.5661312, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(39)->GetValues().coeff(10751), 0.0, 1e-5);
  };
  
  CXXTEST_TEST(ConvertedFromEMGv3)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(HPFFilePathIN + "Set1[Rep2]_v3.emg.hpf");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(),8);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(),60000);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(),2000);
    for (int i = 0 ; i < 8; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), "EMG Ch" + btk::ToString(i+1) + " (V)");
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetUnit(), "V");
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetGain(), btk::Analog::PlusMinus1);
    }
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(0), 1.6327e-5, 1e-9);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(29999), 3.4180e-5, 1e-9);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(12000), 3.4180e-5, 1e-9)
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues().coeff(59999), 4.3945e-5, 1e-9);
  }
};

CXXTEST_SUITE_REGISTRATION(HPFFileReaderTest)
CXXTEST_TEST_REGISTRATION(HPFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(HPFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(HPFFileReaderTest, Run_number_34_VTT_Rep_1DOT6)
CXXTEST_TEST_REGISTRATION(HPFFileReaderTest, ConvertedFromEMGv3)
#endif
