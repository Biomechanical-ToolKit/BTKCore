#ifndef AcquisitionUnitConverterTest_h
#define AcquisitionUnitConverterTest_h

#include <btkPoint.h>
#include <btkAcquisition.h>
#include <btkAcquisitionUnitConverter.h>
#include <btkAcquisitionFileReader.h>
#include <btkForcePlatformsExtractor.h>
#include <btkGroundReactionWrenchFilter.h>

CXXTEST_SUITE(AcquisitionUnitConverterTest)
{ 
  CXXTEST_TEST(NoInputNoConversion)
  {
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->Update();
    btk::Acquisition::Pointer output = uc->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Marker), "mm");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Angle), "deg");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Force), "N");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Moment), "Nmm");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Power), "W");
  };
  
  CXXTEST_TEST(NoInputConversionToMeter)
  {
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->SetUnit(btk::AcquisitionUnitConverter::Length, "m");
    uc->SetUnit(btk::AcquisitionUnitConverter::Moment, "Nm");
    uc->Update();
    btk::Acquisition::Pointer output = uc->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Marker), "mm");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Angle), "deg");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Force), "N");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Moment), "Nmm");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Power), "W");
  };
  
  CXXTEST_TEST(ConversionToMeter)
  {
    double *a, *b;
    btk::Acquisition::Pointer input = btk::Acquisition::New();
    input->Init(2,5);
    btk::Point::Pointer p1 = input->GetPoint(0);
    a = p1->GetValues().data();
    a[0] = 1234.56; a[1] = 34.65; a[2] = 98.08; a[3] = 987.12; a[4] = 654.12;
    btk::Point::Pointer p2 = input->GetPoint(1);
    p2->SetType(btk::Point::Moment);
    b = p2->GetValues().data();
    b[0] = 1234.56; b[1] = 1134.65; b[2] = 1000.54; b[3] = 987.12; b[4] = 1435.896;
    
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->SetInput(input);
    uc->SetUnit(btk::AcquisitionUnitConverter::Length, "m");
    uc->SetUnit(btk::AcquisitionUnitConverter::Moment, "Nm");
    uc->Update();
    btk::Acquisition::Pointer output = uc->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Marker), "m");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Angle), "deg");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Force), "N");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Moment), "Nm");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Power), "W");
    
    p1 = output->GetPoint(0);
    a = p1->GetValues().data();
    p2 = output->GetPoint(1);
    b = p2->GetValues().data();
    
    TS_ASSERT_DELTA(a[0], 1.23456, 1e-10); 
    TS_ASSERT_DELTA(a[1], 0.03465, 1e-10); 
    TS_ASSERT_DELTA(a[2], 0.09808, 1e-10); 
    TS_ASSERT_DELTA(a[3], 0.98712, 1e-10); 
    TS_ASSERT_DELTA(a[4], 0.65412, 1e-10);
    
    TS_ASSERT_DELTA(b[0], 1.23456, 1e-10);
    TS_ASSERT_DELTA(b[1], 1.13465, 1e-10);
    TS_ASSERT_DELTA(b[2], 1.00054, 1e-10);
    TS_ASSERT_DELTA(b[3], 0.98712, 1e-10);
    TS_ASSERT_DELTA(b[4], 1.435896, 1e-10);
  };
  
  CXXTEST_TEST(ConversionForAnalog)
  {
    double *a, *b;
    btk::Acquisition::Pointer input = btk::Acquisition::New();
    input->Init(0,5,2);
    btk::Analog::Pointer a1 = input->GetAnalog(0);
    a1->SetUnit("Nmm");
    a = a1->GetValues().data();
    a[0] = 1234.56; a[1] = 34.65; a[2] = 98.08; a[3] = 987.12; a[4] = 654.12;
    btk::Analog::Pointer a2 = input->GetAnalog(1);
    b = a2->GetValues().data();
    b[0] = 1.56; b[1] = 1.65; b[2] = 1.54; b[3] = 0.98712; b[4] = 1.8964;
    
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->SetInput(input);
    uc->SetUnit(btk::AcquisitionUnitConverter::Length, "m");
    uc->SetUnit(btk::AcquisitionUnitConverter::Moment, "Nm");
    uc->Update();
    btk::Acquisition::Pointer output = uc->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Marker), "m");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Angle), "deg");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Force), "N");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Moment), "Nm");
    TS_ASSERT_EQUALS(output->GetPointUnit(btk::Point::Power), "W");
    
    a1 = output->GetAnalog(0);
    a = a1->GetValues().data();
    a2 = output->GetAnalog(1);
    b = a2->GetValues().data();
    
    TS_ASSERT_DELTA(a[0], 1.23456, 1e-10); 
    TS_ASSERT_DELTA(a[1], 0.03465, 1e-10); 
    TS_ASSERT_DELTA(a[2], 0.09808, 1e-10); 
    TS_ASSERT_DELTA(a[3], 0.98712, 1e-10); 
    TS_ASSERT_DELTA(a[4], 0.65412, 1e-10);
    
    TS_ASSERT_DELTA(b[0], 1.5600, 1e-10);
    TS_ASSERT_DELTA(b[1], 1.6500, 1e-10);
    TS_ASSERT_DELTA(b[2], 1.5400, 1e-10);
    TS_ASSERT_DELTA(b[3], 0.98712, 1e-10);
    TS_ASSERT_DELTA(b[4], 1.8964, 1e-10);
  };
  
  CXXTEST_TEST(ConversionFromFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    //reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->SetInput(acq);
    uc->SetUnit(btk::AcquisitionUnitConverter::Length, "m");
    uc->SetUnit(btk::AcquisitionUnitConverter::Angle, "rad");
    uc->SetUnit(btk::AcquisitionUnitConverter::Moment, "Nm");
    uc->Update();
    
    btk::Acquisition::Pointer acq2 = uc->GetOutput();
    TS_ASSERT_EQUALS(acq2->GetPointUnit(btk::Point::Scalar), "m");
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    
    for (int j = 0 ; j < acq->GetPointNumber() ; ++j)
    {
      double s = 1.0;
      btk::Point::Type t = acq->GetPoint(j)->GetType();
      if ((t == btk::Point::Marker) || (t == btk::Point::Scalar))
        s = 0.001;
      else if (t == btk::Point::Angle)
        s = 1.745329251994e-02;
      else if (t == btk::Point::Moment)
        s = 0.001;
      for (int i = 1 ; i < 50 ; ++i)
      {
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,0) * s, acq2->GetPoint(j)->GetValues()(i,0), 1e-10);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,1) * s, acq2->GetPoint(j)->GetValues()(i,1), 1e-10);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,2) * s, acq2->GetPoint(j)->GetValues()(i,2), 1e-10);
      }
    }
    for (int j = 0 ; j < acq->GetAnalogNumber() ; ++j)
    {
      double s = 1.0;
      std::string unit = acq->GetAnalog(j)->GetUnit();
      if (unit.compare("Nmm") == 0)
        s = 0.001;
      for (int i = 1 ; i < 50 ; ++i)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(j)->GetValues()(i) * s, acq2->GetAnalog(j)->GetValues()(i), 1e-10);
      }
    }
    
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SEG")->GetChild("MARKER_DIAMETER")->GetInfo()->ToDouble(0) * 0.001,
                    acq2->GetMetaData()->GetChild("SEG")->GetChild("MARKER_DIAMETER")->GetInfo()->ToDouble(0), 1e-5);
  };
  
  CXXTEST_TEST(ConversionFromFileCalMatrix_Type4)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample10/TYPE-4.C3D");
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->SetInput(acq);
    uc->SetUnit(btk::AcquisitionUnitConverter::Length, "m");
    uc->SetUnit(btk::AcquisitionUnitConverter::Moment, "Nm");
    
    btk::ForcePlatformsExtractor::Pointer pfe1 = btk::ForcePlatformsExtractor::New();
    pfe1->SetInput(acq);
    btk::ForcePlatformsExtractor::Pointer pfe2 = btk::ForcePlatformsExtractor::New();
    pfe2->SetInput(uc->GetOutput());
    
    btk::GroundReactionWrenchFilter::Pointer grwf1 = btk::GroundReactionWrenchFilter::New();
    grwf1->SetInput(pfe1->GetOutput());
    grwf1->Update();
    btk::Wrench::Pointer grw1 = grwf1->GetOutput()->GetItem(0);
    
    btk::GroundReactionWrenchFilter::Pointer grwf2 = btk::GroundReactionWrenchFilter::New();
    grwf2->SetInput(pfe2->GetOutput());
    grwf2->Update();
    btk::Wrench::Pointer grw2 = grwf2->GetOutput()->GetItem(0);
   
    // NaN in the computation of the moment! 
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(0)->GetChannel(0)->GetValues(), pfe1->GetOutput()->GetItem(0)->GetChannel(0)->GetValues(), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(0)->GetChannel(1)->GetValues(), pfe1->GetOutput()->GetItem(0)->GetChannel(1)->GetValues(), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(0)->GetChannel(2)->GetValues(), pfe1->GetOutput()->GetItem(0)->GetChannel(2)->GetValues(), 1e-15);
    // Frame #21: NaN?
    TS_ASSERT((pfe2->GetOutput()->GetItem(0)->GetChannel(3)->GetValues()-(pfe1->GetOutput()->GetItem(0)->GetChannel(3)->GetValues() * 0.001)).block(0,0,20,1).rowwise().norm().sum() < 1e-5);
    TS_ASSERT((pfe2->GetOutput()->GetItem(0)->GetChannel(4)->GetValues()-(pfe1->GetOutput()->GetItem(0)->GetChannel(4)->GetValues() * 0.001)).block(0,0,20,1).rowwise().norm().sum() < 1e-5);
    TS_ASSERT((pfe2->GetOutput()->GetItem(0)->GetChannel(5)->GetValues()-(pfe1->GetOutput()->GetItem(0)->GetChannel(5)->GetValues() * 0.001)).block(0,0,20,1).rowwise().norm().sum() < 1e-5);
    TS_ASSERT((grw2->GetPosition()->GetValues() - (grw1->GetPosition()->GetValues() * 0.001)).block(0,0,20,3).rowwise().norm().sum() < 1e-5);
    TS_ASSERT((grw2->GetMoment()->GetValues() - (grw1->GetMoment()->GetValues() * 0.001)).block(0,0,20,3).rowwise().norm().sum() < 1e-5);
    TS_ASSERT_EIGEN_DELTA(grw2->GetForce()->GetValues(), grw1->GetForce()->GetValues(), 1e-9);
  };
  
  CXXTEST_TEST(ConversionFromFileCalMatrix_Type4a)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample10/type-4a.c3d");
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    btk::AcquisitionUnitConverter::Pointer uc = btk::AcquisitionUnitConverter::New();
    uc->SetInput(acq);
    uc->SetUnit(btk::AcquisitionUnitConverter::Length, "m");
    uc->SetUnit(btk::AcquisitionUnitConverter::Moment, "Nm");
    
    btk::ForcePlatformsExtractor::Pointer pfe1 = btk::ForcePlatformsExtractor::New();
    pfe1->SetInput(acq);
    btk::ForcePlatformsExtractor::Pointer pfe2 = btk::ForcePlatformsExtractor::New();
    pfe2->SetInput(uc->GetOutput());
    
    btk::GroundReactionWrenchFilter::Pointer grwf1 = btk::GroundReactionWrenchFilter::New();
    grwf1->SetInput(pfe1->GetOutput());
    grwf1->Update();
    
    btk::GroundReactionWrenchFilter::Pointer grwf2 = btk::GroundReactionWrenchFilter::New();
    grwf2->SetInput(pfe2->GetOutput());
    grwf2->Update();
    
    // FP1
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(0)->GetChannel(0)->GetValues(), pfe1->GetOutput()->GetItem(0)->GetChannel(0)->GetValues(), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(0)->GetChannel(1)->GetValues(), pfe1->GetOutput()->GetItem(0)->GetChannel(1)->GetValues(), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(0)->GetChannel(2)->GetValues(), pfe1->GetOutput()->GetItem(0)->GetChannel(2)->GetValues(), 1e-15);
    TS_ASSERT((pfe2->GetOutput()->GetItem(0)->GetChannel(3)->GetValues()-(pfe1->GetOutput()->GetItem(0)->GetChannel(3)->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT((pfe2->GetOutput()->GetItem(0)->GetChannel(4)->GetValues()-(pfe1->GetOutput()->GetItem(0)->GetChannel(4)->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT((pfe2->GetOutput()->GetItem(0)->GetChannel(5)->GetValues()-(pfe1->GetOutput()->GetItem(0)->GetChannel(5)->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    btk::Wrench::Pointer grw1 = grwf1->GetOutput()->GetItem(0);
    btk::Wrench::Pointer grw2 = grwf2->GetOutput()->GetItem(0);
    TS_ASSERT((grw2->GetPosition()->GetValues() - (grw1->GetPosition()->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT((grw2->GetMoment()->GetValues() - (grw1->GetMoment()->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT_EIGEN_DELTA(grw2->GetForce()->GetValues(), grw1->GetForce()->GetValues(), 1e-9);
    
    // FP2
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(1)->GetChannel(0)->GetValues(), pfe1->GetOutput()->GetItem(1)->GetChannel(0)->GetValues(), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(1)->GetChannel(1)->GetValues(), pfe1->GetOutput()->GetItem(1)->GetChannel(1)->GetValues(), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pfe2->GetOutput()->GetItem(1)->GetChannel(2)->GetValues(), pfe1->GetOutput()->GetItem(1)->GetChannel(2)->GetValues(), 1e-15);
    TS_ASSERT((pfe2->GetOutput()->GetItem(1)->GetChannel(3)->GetValues()-(pfe1->GetOutput()->GetItem(1)->GetChannel(3)->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT((pfe2->GetOutput()->GetItem(1)->GetChannel(4)->GetValues()-(pfe1->GetOutput()->GetItem(1)->GetChannel(4)->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT((pfe2->GetOutput()->GetItem(1)->GetChannel(5)->GetValues()-(pfe1->GetOutput()->GetItem(1)->GetChannel(5)->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    grw1 = grwf1->GetOutput()->GetItem(1);
    grw2 = grwf2->GetOutput()->GetItem(1);
    TS_ASSERT((grw2->GetPosition()->GetValues() - (grw1->GetPosition()->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT((grw2->GetMoment()->GetValues() - (grw1->GetMoment()->GetValues() * 0.001)).rowwise().norm().sum() < 1e-3);
    TS_ASSERT_EIGEN_DELTA(grw2->GetForce()->GetValues(), grw1->GetForce()->GetValues(), 1e-9);
  };
};

CXXTEST_SUITE_REGISTRATION(AcquisitionUnitConverterTest)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, NoInputNoConversion)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, NoInputConversionToMeter)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, ConversionToMeter)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, ConversionForAnalog)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, ConversionFromFile)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, ConversionFromFileCalMatrix_Type4)
CXXTEST_TEST_REGISTRATION(AcquisitionUnitConverterTest, ConversionFromFileCalMatrix_Type4a)
#endif
