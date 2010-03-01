#ifndef ModelMotionFilterTest_h
#define ModelMotionFilterTest_h

#include <btkModelMotionFilter.h>
#include <btkAcquisitionFileReader.h>
#include <btkBasicMotionBuilder.h>
#include <btkUpperLimbModelConfigurator.h>
#include <btkModelCalibrationFilter.h>
#include <btkStaticCalibrationProcedure.h>
#include <btkModelJCSKinematicFilter.h>
#include <btkSubject.h>

CXXTEST_SUITE(ModelJCSKinematicFilterTest)
{
  CXXTEST_TEST(StaticUpperLimb)
  {
    // Acquisiton
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "UpperLimbStatic.trc");
    btk::Acquisition::Pointer acq = reader->GetOutput();
    // Model
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Both);
    // Subject
    btk::Subject::Pointer subject = btk::Subject::New();
    subject->SetSex(btk::Subject::Male);
    subject->SetHeight(1760.0); // mm
    subject->SetHeight(72.0); // kg
    // Translator
    btk::LandmarksTranslator::Pointer tr = btk::LandmarksTranslator::New();
    tr->SetLandmark("C7", "C7");
    tr->SetLandmark("T8", "T8");
    tr->SetLandmark("SS", "IJ");
    tr->SetLandmark("XP", "PX");
    tr->SetLandmark("AC_L", "L.Acromion");
    tr->SetLandmark("AA_L", "L.AngulusAcromio");
    tr->SetLandmark("GT_L", "L.Biceps");
    tr->SetLandmark("LHE_L", "L.Epi.Lat");
    tr->SetLandmark("MHE_L", "L.Epi.Med");
    tr->SetLandmark("US_L", "L.Wrist.Ulna");
    tr->SetLandmark("RS_L", "L.Wrist.Rad");
    tr->SetLandmark("MH5_L", "L.D5M1");
    tr->SetLandmark("MH2_L", "L.D2M1");
    tr->SetLandmark("AC_R", "R.Acromion");
    tr->SetLandmark("AA_R", "R.AngulusAcromio");
    tr->SetLandmark("GT_R", "R.Biceps");
    tr->SetLandmark("LHE_R", "R.Epi.Lat");
    tr->SetLandmark("MHE_R", "R.Epi.Med");
    tr->SetLandmark("US_R", "R.Wrist.Ulna");
    tr->SetLandmark("RS_R", "R.Wrist.Rad");
    tr->SetLandmark("MH5_R", "R.D5M1");
    tr->SetLandmark("MH2_R", "R.D2M1");
    // Calibration
    btk::StaticCalibrationProcedure::Pointer scp = btk::StaticCalibrationProcedure::New();
    btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
    cal->SetAcquisition(acq);
    cal->SetModel(model);
    cal->SetSubject(subject);
    cal->SetLandmarksTranslator(tr);
    cal->SetCalibrationProcedure(scp);
    // Motion
    btk::ModelMotionFilter::Pointer bsmd = btk::ModelMotionFilter::New();
    bsmd->SetAcquisition(acq);
    bsmd->SetModel(cal->GetOutput());
    bsmd->SetLandmarksTranslator(tr);
    bsmd->SetMotionBuilder(btk::BasicMotionBuilder::New());
    // Kinematics
    btk::ModelJCSKinematicFilter::Pointer jcsf = btk::ModelJCSKinematicFilter::New();
    jcsf->SetInput(bsmd->GetOutput());
    jcsf->SetSequenceJCS("LWrist", btk::SequenceJCS::AxisZ, btk::SequenceJCS::AxisY, btk::SequenceJCS::AxisX);
    jcsf->SetSequenceJCS("RWrist", btk::SequenceJCS::AxisZ, btk::SequenceJCS::AxisY, btk::SequenceJCS::AxisX);
    btk::Acquisition::Pointer output = jcsf->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetPointNumber(), 7);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 146);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 146);
    
    Eigen::Matrix<double,1,3> row;
    // Left shoulder angles #22
    row << -14.280299240736074, 69.427283972842730, 52.838308835108698;
    TS_ASSERT(output->GetPoint("LShoulderAngles")->GetValues().row(22).isApprox(row, 1e-8));
    // Left elbow angles #75
    row << 55.4874205000540, 7.0913288070541, -125.8854440811594;
    TS_ASSERT(output->GetPoint("LElbowAngles")->GetValues().row(75).isApprox(row, 1e-8));
    // Left wrist angles #126
    row << 20.486435656144469, 23.050571399986694, -12.364210415360624;
    TS_ASSERT(output->GetPoint("LWristAngles")->GetValues().row(126).isApprox(row, 1e-8));
    // Right shoulder angles #99
    row << -27.492131443929544, -65.774626662188481, -50.971190329038215;
    TS_ASSERT(output->GetPoint("RShoulderAngles")->GetValues().row(99).isApprox(row, 1e-8));
    // Right elbow angles #33
    row << 62.3409805537806, -9.6279405204294, 114.0440863078523;
    TS_ASSERT(output->GetPoint("RElbowAngles")->GetValues().row(33).isApprox(row, 1e-8));
    // Right wrist angles #145
    row << 6.995882978659257, -11.388780067630307, 9.746227693435785;
    TS_ASSERT(output->GetPoint("RWristAngles")->GetValues().row(145).isApprox(row, 1e-8));
    // Thorax angles #0
    row << -109.9300720909064, 84.1764610476819, 112.1812594245926;
    TS_ASSERT(output->GetPoint("ThoraxAngles")->GetValues().row(0).isApprox(row, 1e-8));
  };
};

CXXTEST_SUITE_REGISTRATION(ModelJCSKinematicFilterTest)
CXXTEST_TEST_REGISTRATION(ModelJCSKinematicFilterTest, StaticUpperLimb)
#endif
