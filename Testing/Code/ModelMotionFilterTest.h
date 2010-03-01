#ifndef ModelMotionFilterTest_h
#define ModelMotionFilterTest_h

#include <btkModelMotionFilter.h>
#include <btkAcquisitionFileReader.h>
#include <btkBasicMotionBuilder.h>
#include <btkUpperLimbModelConfigurator.h>
#include <btkModelCalibrationFilter.h>
#include <btkStaticCalibrationProcedure.h>
#include <btkSubject.h>

CXXTEST_SUITE(ModelMotionFilterTest)
{
  CXXTEST_TEST(BasicUpperLimb)
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
    btk::Model::Pointer output = bsmd->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetMotionFrameNumber(), acq->GetPointFrameNumber());
    
    btk::Segment::Pointer seg;
    btk::Frame::Axis TFx, TFy, TFz;
    btk::Frame::Origin TFo;
    btk::Frame frame, af;
    // Trunk, frame #44
    seg = output->GetSegment(0);
    TFx << 0.9989759545, 0.0287431721, -0.0349409834;
    TFy << 0.0358654068, -0.0323005241, 0.9988344952;
    TFz << 0.0275810598, -0.9990648158, -0.0332983325;
    TFo = acq->GetPoint("C7")->GetValues().row(44);
    af.GetMatrix() << 0.9947868968, 0.0981749206, 0.0275810598, 14.0739380377,
                      0.0307040361, -0.0304426669, -0.9990648158, -112.1111774964,
                      -0.0972434680, 0.9947034377, -0.0332983325, 1073.1328298614,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(44);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(44).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    // Left arm, frame #102
    seg = output->GetSegment(1);
    TFx << 0.7805359046, 0.3790387982, 0.4970847926;
    TFy << 0.0317366197, -0.8181973131, 0.5740609234;
    TFz << 0.6243048042, -0.4322993711, -0.6506617902;
    TFo = acq->GetPoint("L.Biceps")->GetValues().row(102);
    af.GetMatrix() << 0.7973677183, 0.1556619676, 0.5830729575, -11.2920911653,
                      0.3022550714, -0.9392584763, -0.1625896261, 70.2641784535,
                      0.5223471965, 0.3058804775, -0.7959840072, 1041.7690843507,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(102);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(102).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    // Left forearm, frame #5
    seg = output->GetSegment(2);
    TFx << 0.1823621254, 0.3037176942, -0.9351468427;
    TFy << -0.5103727504, -0.7836871488, -0.3540538214;
    TFz << -0.8403949731, 0.5418394735, 0.0120943808;
    TFo = ((acq->GetPoint("L.Epi.Lat")->GetValues() + acq->GetPoint("L.Epi.Med")->GetValues()) / 2.0).row(5);
    af.GetMatrix() << 0.1823621254, -0.5103727504, -0.8403949731, -45.2054200000,
                      0.3037176942, -0.7836871488, 0.5418394735, 317.2064350000,
                      -0.9351468427, -0.3540538214, 0.0120943808, 966.7606200000,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(5);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(5).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    // Left hand, frame #32
    seg = output->GetSegment(3);
    TFx << 0.3370520349, -0.2541281133, -0.9065400310;
    TFy << -0.3805269349, -0.9175009438, 0.1157206548;
    TFz << -0.8611592057, 0.3059590172, -0.4059481522;
    TFo = ((acq->GetPoint("L.Wrist.Ulna")->GetValues() + acq->GetPoint("L.Wrist.Rad")->GetValues()) / 2.0).row(32);
    af.GetMatrix() << 0.3370520349, -0.3805269349, -0.8611592057, 83.9074950000,
                      -0.2541281133, -0.9175009438, 0.3059590172, 519.8002450000,
                      -0.9065400310, 0.1157206548, -0.4059481522, 1053.8471700000,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(32);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(32).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    // Right arm, frame #67
    seg = output->GetSegment(4);
    TFx << 0.9399457534, -0.2332588751, 0.2491832214;
    TFy << 0.0340092791, 0.7904199189, 0.6116205692;
    TFz << -0.3396253076, -0.5664156150, 0.7508848124;
    TFo = acq->GetPoint("R.Biceps")->GetValues().row(67);
    af.GetMatrix() << 0.9242784125, 0.2038885628, -0.3227055473, -17.7664536521,
                      -0.3029628409, 0.9061167864, -0.2952386939, -282.4730591531,
                      0.2322131205, 0.3706505407, 0.8992748431, 1020.2983491750,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(67);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(67).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    // Right forearm, frame #90
    seg = output->GetSegment(5);
    TFx << 0.1719042574, -0.1061332154, -0.9793797358;
    TFy << -0.6484765074, 0.7362036602, -0.1936036934;
    TFz << 0.7415707287, 0.6683860496, 0.0577316467;
    TFo = ((acq->GetPoint("R.Epi.Lat")->GetValues() + acq->GetPoint("R.Epi.Med")->GetValues()) / 2.0).row(90);
    af.GetMatrix() << 0.1719042574, -0.6484765074, 0.7415707287, -74.2896050000,
                      -0.1061332154, 0.7362036602, 0.6683860496, -535.5918300000,
                      -0.9793797358, -0.1936036934, 0.0577316467, 919.1575050000,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(90);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(90).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    // Righ hand, frame #145
    seg = output->GetSegment(6);
    TFx << 0.2356765711, 0.1088597395, -0.9657153364;
    TFy << -0.5420506897, 0.8395018818, -0.0376515629;
    TFz << 0.8066211029, 0.5323402554, 0.2568584218;
    TFo = ((acq->GetPoint("R.Wrist.Ulna")->GetValues() + acq->GetPoint("R.Wrist.Rad")->GetValues()) / 2.0).row(145);
    af.GetMatrix() << 0.2356765711, -0.5420506897, 0.8066211029, 96.5360600000,
                      0.1088597395, 0.8395018818, 0.5323402554, -724.7153950000,
                      -0.9657153364, -0.0376515629, 0.2568584218, 974.8815600000,
                      0.0, 0.0, 0.0, 1.0;
    frame = seg->GetMotion()->GetFrame(145);
    TS_ASSERT(frame.GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(frame.GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(frame.GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(frame.GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetAFMotion()->GetFrame(145).GetMatrix().isApprox(af.GetMatrix(), 1e-8));  
    //std::cout << std::endl << seg->GetMotion()->GetFrame(44).GetAxisX().transpose() << std::endl;
  };
};

CXXTEST_SUITE_REGISTRATION(ModelMotionFilterTest)
CXXTEST_TEST_REGISTRATION(ModelMotionFilterTest, BasicUpperLimb)
#endif
