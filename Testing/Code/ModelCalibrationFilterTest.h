#ifndef StaticCalibrationProcedureTest_h
#define StaticCalibrationProcedureTest_h

#include <btkModelCalibrationFilter.h>
#include <btkStaticCalibrationProcedure.h>
#include <btkUpperLimbModelConfigurator.h>
#include <btkAcquisitionFileReader.h>
#include <btkLandmarksTranslator.h>
#include <Eigen/Array>

CXXTEST_SUITE(ModelCalibrationFilterTest)
{
  CXXTEST_TEST(NoInputs)
  {
    btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
    btk::Model::Pointer output = cal->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetLabel(), "");
    TS_ASSERT_EQUALS(output->GetDescription(), "");
    TS_ASSERT_EQUALS(output->GetSegmentNumber(), 0);
  };
    
  CXXTEST_TEST(StaticUpperLimbModelLeftMissingLandmark)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Left, "Foo", "Simple description");
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(3,1);
    acq->GetPoint(0)->SetLabel("Test1");
    acq->GetPoint(1)->SetLabel("Test2");
    acq->GetPoint(2)->SetLabel("Test3");
    btk::Subject::Pointer subject = btk::Subject::New();
    btk::StaticCalibrationProcedure::Pointer scp = btk::StaticCalibrationProcedure::New();
    btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
    cal->SetAcquisition(acq);
    cal->SetModel(model);
    cal->SetSubject(subject);
    cal->SetCalibrationProcedure(scp);
    btk::Model::Pointer output = cal->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetLabel(), "Foo");
    TS_ASSERT_EQUALS(output->GetDescription(), "Simple description");
    TS_ASSERT_EQUALS(output->GetSegmentNumber(), 4);
    TS_ASSERT_EQUALS(output->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(output->GetSegment(1)->GetLabel(), "LArm");
    TS_ASSERT_EQUALS(output->GetSegment(2)->GetLabel(), "LForearm");
    TS_ASSERT_EQUALS(output->GetSegment(3)->GetLabel(), "LHand");
    btk::Frame::Axis u, w;
    u << 1.0, 0.0, 0.0;
    w << 0.0, 0.0, 1.0;
    TS_ASSERT(output->GetSegment(0)->GetMotion()->GetFrame(0).GetAxisX().isApprox(u));
    TS_ASSERT(output->GetSegment(0)->GetMotion()->GetFrame(0).GetAxisZ().isApprox(w));
    TS_ASSERT(output->GetSegment(1)->GetMotion()->GetFrame(0).GetAxisX().isApprox(u));
    TS_ASSERT(output->GetSegment(1)->GetMotion()->GetFrame(0).GetAxisZ().isApprox(w));
    TS_ASSERT(output->GetSegment(2)->GetMotion()->GetFrame(0).GetAxisX().isApprox(u));
    TS_ASSERT(output->GetSegment(2)->GetMotion()->GetFrame(0).GetAxisZ().isApprox(w));
    TS_ASSERT(output->GetSegment(3)->GetMotion()->GetFrame(0).GetAxisX().isApprox(u));
    TS_ASSERT(output->GetSegment(3)->GetMotion()->GetFrame(0).GetAxisZ().isApprox(w));
  };
  
  CXXTEST_TEST(StaticUpperLimbModelLeftNoTranslator)
  {
    btk::Subject::Pointer subject = btk::Subject::New();
    subject->SetSex(btk::Subject::Male);
    subject->SetHeight(1760.0); // mm
    subject->SetHeight(72.0); // kg
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Left, "Foo", "Simple description");
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(13,1);
    acq->GetPoint(0)->SetLabel("RS_L"); acq->GetPoint(0)->GetValues().row(0) << 11.5, 125.0, 20.0;
    acq->GetPoint(1)->SetLabel("MHE_L"); acq->GetPoint(1)->GetValues().row(0) << 33.75, 145.5, 32.2;
    acq->GetPoint(2)->SetLabel("MH5_L"); acq->GetPoint(2)->GetValues().row(0) << 23.75, 200.1, 43.2;
    acq->GetPoint(3)->SetLabel("LHE_L"); acq->GetPoint(3)->GetValues().row(0) << 21.5, 105.0, 19.5;
    acq->GetPoint(4)->SetLabel("AC_L"); acq->GetPoint(4)->GetValues().row(0) << 13.5, 114.20, 21.8;
    acq->GetPoint(5)->SetLabel("MH2_L"); acq->GetPoint(5)->GetValues().row(0) << 41.75, 95.75, 27.4;
    acq->GetPoint(6)->SetLabel("AA_L"); acq->GetPoint(6)->GetValues().row(0) << 14.5, 100.0, 13.5;
    acq->GetPoint(7)->SetLabel("US_L"); acq->GetPoint(7)->GetValues().row(0) << 51.25, 132.3, 24.4;
    acq->GetPoint(8)->SetLabel("GT_L"); acq->GetPoint(8)->GetValues().row(0) << 16.0, 111.8, 30.0;
    acq->GetPoint(9)->SetLabel("T8"); acq->GetPoint(9)->GetValues().row(0) << 6.0, 91.5, 23.2;
    acq->GetPoint(10)->SetLabel("XP"); acq->GetPoint(10)->GetValues().row(0) << 19.0, 131.7, 25.0;
    acq->GetPoint(11)->SetLabel("SS"); acq->GetPoint(11)->GetValues().row(0) << 25.4, 99.35, 32.27;
    acq->GetPoint(12)->SetLabel("C7"); acq->GetPoint(12)->GetValues().row(0) << 18.2, 124.2, 12.12;
    btk::StaticCalibrationProcedure::Pointer scp = btk::StaticCalibrationProcedure::New();
    btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
    cal->SetAcquisition(acq);
    cal->SetModel(model);
    cal->SetSubject(subject);
    cal->SetCalibrationProcedure(scp);
    btk::Model::Pointer output = cal->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetLabel(), "Foo");
    TS_ASSERT_EQUALS(output->GetDescription(), "Simple description");
    TS_ASSERT_EQUALS(output->GetSegmentNumber(), 4);
    TS_ASSERT_EQUALS(output->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(output->GetSegment(1)->GetLabel(), "LArm");
    TS_ASSERT_EQUALS(output->GetSegment(2)->GetLabel(), "LForearm");
    TS_ASSERT_EQUALS(output->GetSegment(3)->GetLabel(), "LHand");
    //btk::Frame::Axis xTF;
    //xTF << 0.219558628485959, -0.757782210816123, 0.614459217221122;
    //TS_ASSERT(output->GetSegment(0)->GetMotion()->GetFrame(0).GetAxisX().isApprox(xTF));
    TS_ASSERT(output->GetSegment(0)->GetMotion()->GetFrame(0).GetOrigin().isApprox(acq->GetPoint(12)->GetValues().row(0).transpose()));
  };
  
  CXXTEST_TEST(StaticUpperLimbModelBothWithTranslator)
  {
    // Acquisiton
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "UpperLimbStatic.trc");
    btk::Acquisition::Pointer acq = reader->GetOutput();
    // Subject
    btk::Subject::Pointer subject = btk::Subject::New();
    subject->SetSex(btk::Subject::Male);
    subject->SetHeight(1760.0); // mm
    subject->SetHeight(72.0); // kg
    // Model
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Both);
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
    btk::Model::Pointer output = cal->GetOutput();
    output->Update();
    
    btk::Segment::Pointer seg;
    btk::Frame::Axis TFx, TFy, TFz, AFx, AFy, AFz;
    btk::Frame::Origin AFo, TFo;
    // Trunk
    TFo << -72.0653164384, -113.0161271918, 1097.4540036301;
    TFx << 0.9990208699, 0.0280923768, -0.0341777682;
    TFy << 0.0349023381, -0.0257198909, 0.9990597149;
    TFz << 0.0271869135, -0.9992743895, -0.0266751974;
    AFo << 86.2581302099, -21.0216470741, 0.0000000000;
    AFx << 0.9980485036, -0.0624434499, -0.0000000000;
    AFy << 0.0624434499, 0.9980485036, -0.0000000000;
    AFz << -0.0000000000, 0.0000000000, 1.0000000000;
    seg = output->GetSegment(0);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Left Arm
    TFo << -45.6265100000, 187.9285986301, 1052.7102602055;
    TFx << 0.7873889638, 0.3708187430, 0.4924551548;
    TFy << 0.0292004356, -0.8203869543, 0.5710626759;
    TFz << 0.6157645282, -0.4352685436, -0.6567917027;
    AFo << -21.8186956982, 92.5920080279, 80.4826689346;
    AFx << 0.9965913801, 0.0778605827, 0.0272644580;
    AFy << -0.0824671156, 0.9490329757, 0.3041966237;
    AFz << -0.0021899433, -0.3054081543, 0.9522190207;
    seg = output->GetSegment(1);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Left Foream
    TFo << -50.2547181507, 317.9582202740, 962.1980168151;
    TFx << 0.1972819089, 0.2883047292, -0.9369953210;
    TFy << -0.4986494500, -0.7933925572, -0.3491088316;
    TFz << -0.8440548409, 0.5361050581, -0.0127590035;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(2);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Left Hand
    TFo << 78.626423287671, 0523.018786130137, 1052.428828664384;
    TFx << 0.3471697623, -0.2433139491, -0.9056884002;
    TFy << -0.3689412885, -0.9233163545, 0.1066266158;
    TFz << -0.8621806549, 0.2971283085, -0.4103160812;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(3);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Right Arm
    TFo << -68.5893408904, -407.0302015068, 1015.8376139041;
    TFx << 0.9399805099, -0.2335915663, 0.2487400674;
    TFy << 0.0337479841, 0.7890220092, 0.6134373175;
    TFz << -0.3395551716, -0.5682246467, 0.7495485551;
    AFo << 20.3242875484, 101.9281579640, -85.0997106446;
    AFx << 0.9973039536, -0.0660075007, 0.0320598491;
    AFy << 0.0726443022, 0.9498443546, -0.3041685510;
    AFz << -0.0103744608, 0.3056774639, 0.9520785990;
    seg = output->GetSegment(4);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Right Foream
    TFo << -74.1178521575343, -536.2858352739726, 915.3458299999995;
    TFx << 0.1644127388, -0.0994137426, -0.9813691248;
    TFy << -0.6545129764, 0.7333338788, -0.1839407130;
    TFz << 0.7379574615, 0.6725610232, 0.0555018469;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(5);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    //std::cout << std::endl << seg->GetTF() << std::endl;
    // Right Hand
    TFo << 95.8399489041096, -726.7111102054796, 963.1098319863016;
    TFx << 0.2414576828, 0.1458918418, -0.9593819666;
    TFy << -0.5551365258, 0.8316537211, -0.0132486189;
    TFz << 0.7959407171, 0.5357869527, 0.2817990706;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(6);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
  }
  
  CXXTEST_TEST(StaticUpperLimbModelBothLabelRenamedNoTranslator)
  {
    // Acquisiton
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "UpperLimbStatic.trc");
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->Update();
    reader.reset(); // Delete the reader
    // Renaming markers' label to fit labels used in the model
    acq->GetPoint("C7")->SetLabel("C7");
    acq->GetPoint("T8")->SetLabel("T8");
    acq->GetPoint("IJ")->SetLabel("SS");
    acq->GetPoint("PX")->SetLabel("XP");
    acq->GetPoint("L.Acromion")->SetLabel("AC_L");
    acq->GetPoint("L.AngulusAcromio")->SetLabel("AA_L");
    acq->GetPoint("L.Biceps")->SetLabel("GT_L");
    acq->GetPoint("L.Epi.Lat")->SetLabel("LHE_L");
    acq->GetPoint("L.Epi.Med")->SetLabel("MHE_L");
    acq->GetPoint("L.Wrist.Ulna")->SetLabel("US_L");
    acq->GetPoint("L.Wrist.Rad")->SetLabel("RS_L");
    acq->GetPoint("L.D5M1")->SetLabel("MH5_L");
    acq->GetPoint("L.D2M1")->SetLabel("MH2_L");
    acq->GetPoint("R.Acromion")->SetLabel("AC_R");
    acq->GetPoint("R.AngulusAcromio")->SetLabel("AA_R");
    acq->GetPoint("R.Biceps")->SetLabel("GT_R");
    acq->GetPoint("R.Epi.Lat")->SetLabel("LHE_R");
    acq->GetPoint("R.Epi.Med")->SetLabel("MHE_R");
    acq->GetPoint("R.Wrist.Ulna")->SetLabel("US_R");
    acq->GetPoint("R.Wrist.Rad")->SetLabel("RS_R");
    acq->GetPoint("R.D5M1")->SetLabel("MH5_R");
    acq->GetPoint("R.D2M1")->SetLabel("MH2_R");
    // Subject
    btk::Subject::Pointer subject = btk::Subject::New();
    subject->SetSex(btk::Subject::Male);
    subject->SetHeight(1760.0); // mm
    subject->SetHeight(72.0); // kg
    // Model
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Both);
    // Calibration
    btk::StaticCalibrationProcedure::Pointer scp = btk::StaticCalibrationProcedure::New();
    btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
    cal->SetAcquisition(acq);
    cal->SetModel(model);
    cal->SetSubject(subject);
    cal->SetCalibrationProcedure(scp);
    btk::Model::Pointer output = cal->GetOutput();
    output->Update();
    
    btk::Segment::Pointer seg;
    btk::Frame::Axis TFx, TFy, TFz, AFx, AFy, AFz;
    btk::Frame::Origin AFo, TFo;
    // Trunk
    TFo << -72.0653164384, -113.0161271918, 1097.4540036301;
    TFx << 0.9990208699, 0.0280923768, -0.0341777682;
    TFy << 0.0349023381, -0.0257198909, 0.9990597149;
    TFz << 0.0271869135, -0.9992743895, -0.0266751974;
    AFo << 86.2581302099, -21.0216470741, 0.0000000000;
    AFx << 0.9980485036, -0.0624434499, -0.0000000000;
    AFy << 0.0624434499, 0.9980485036, -0.0000000000;
    AFz << -0.0000000000, 0.0000000000, 1.0000000000;
    seg = output->GetSegment(0);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Left Arm
    TFo << -45.6265100000, 187.9285986301, 1052.7102602055;
    TFx << 0.7873889638, 0.3708187430, 0.4924551548;
    TFy << 0.0292004356, -0.8203869543, 0.5710626759;
    TFz << 0.6157645282, -0.4352685436, -0.6567917027;
    AFo << -21.8186956982, 92.5920080279, 80.4826689346;
    AFx << 0.9965913801, 0.0778605827, 0.0272644580;
    AFy << -0.0824671156, 0.9490329757, 0.3041966237;
    AFz << -0.0021899433, -0.3054081543, 0.9522190207;
    seg = output->GetSegment(1);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Left Foream
    TFo << -50.2547181507, 317.9582202740, 962.1980168151;
    TFx << 0.1972819089, 0.2883047292, -0.9369953210;
    TFy << -0.4986494500, -0.7933925572, -0.3491088316;
    TFz << -0.8440548409, 0.5361050581, -0.0127590035;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(2);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Left Hand
    TFo << 78.626423287671, 0523.018786130137, 1052.428828664384;
    TFx << 0.3471697623, -0.2433139491, -0.9056884002;
    TFy << -0.3689412885, -0.9233163545, 0.1066266158;
    TFz << -0.8621806549, 0.2971283085, -0.4103160812;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(3);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Right Arm
    TFo << -68.5893408904, -407.0302015068, 1015.8376139041;
    TFx << 0.9399805099, -0.2335915663, 0.2487400674;
    TFy << 0.0337479841, 0.7890220092, 0.6134373175;
    TFz << -0.3395551716, -0.5682246467, 0.7495485551;
    AFo << 20.3242875484, 101.9281579640, -85.0997106446;
    AFx << 0.9973039536, -0.0660075007, 0.0320598491;
    AFy << 0.0726443022, 0.9498443546, -0.3041685510;
    AFz << -0.0103744608, 0.3056774639, 0.9520785990;
    seg = output->GetSegment(4);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Right Foream
    TFo << -74.1178521575343, -536.2858352739726, 915.3458299999995;
    TFx << 0.1644127388, -0.0994137426, -0.9813691248;
    TFy << -0.6545129764, 0.7333338788, -0.1839407130;
    TFz << 0.7379574615, 0.6725610232, 0.0555018469;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(5);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    //std::cout << std::endl << seg->GetTF() << std::endl;
    // Right Hand
    TFo << 95.8399489041096, -726.7111102054796, 963.1098319863016;
    TFx << 0.2414576828, 0.1458918418, -0.9593819666;
    TFy << -0.5551365258, 0.8316537211, -0.0132486189;
    TFz << 0.7959407171, 0.5357869527, 0.2817990706;
    AFo << 0.0, 0.0, 0.0;
    AFx << 1.0, 0.0, 0.0;
    AFy << 0.0, 1.0, 0.0;
    AFz << 0.0, 0.0, 1.0;
    seg = output->GetSegment(6);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
  }
  
  CXXTEST_TEST(StaticUpperLimbModelOnlyTwoSegments)
  {
    // Acquisiton
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "UpperLimbStatic.trc");
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->Update();
    reader.reset(); // Delete the reader
    // Renaming markers' label to fit labels used in the model
    acq->GetPoint("C7")->SetLabel("C7");
    acq->GetPoint("T8")->SetLabel("T8");
    acq->GetPoint("IJ")->SetLabel("SS");
    acq->GetPoint("PX")->SetLabel("XP");
    acq->GetPoint("L.Acromion")->SetLabel("AC_L");
    acq->GetPoint("L.AngulusAcromio")->SetLabel("AA_L");
    acq->GetPoint("L.Biceps")->SetLabel("GT_L");
    acq->GetPoint("L.Epi.Lat")->SetLabel("LHE_L");
    acq->GetPoint("L.Epi.Med")->SetLabel("MHE_L");
    acq->GetPoint("L.Wrist.Ulna")->SetLabel("US_L");
    acq->GetPoint("L.Wrist.Rad")->SetLabel("RS_L");
    acq->GetPoint("L.D5M1")->SetLabel("MH5_L");
    acq->GetPoint("L.D2M1")->SetLabel("MH2_L");
    acq->GetPoint("R.Acromion")->SetLabel("AC_R");
    acq->GetPoint("R.AngulusAcromio")->SetLabel("AA_R");
    acq->GetPoint("R.Biceps")->SetLabel("GT_R");
    acq->GetPoint("R.Epi.Lat")->SetLabel("LHE_R");
    acq->GetPoint("R.Epi.Med")->SetLabel("MHE_R");
    acq->GetPoint("R.Wrist.Ulna")->SetLabel("US_R");
    acq->GetPoint("R.Wrist.Rad")->SetLabel("RS_R");
    acq->GetPoint("R.D5M1")->SetLabel("MH5_R");
    acq->GetPoint("R.D2M1")->SetLabel("MH2_R");
    // Subject
    btk::Subject::Pointer subject = btk::Subject::New();
    subject->SetSex(btk::Subject::Male);
    subject->SetHeight(1760.0); // mm
    subject->SetHeight(72.0); // kg
    // Model
    btk::Model::Pointer model = btk::Model::New();
    model->AppendSegment(btk::Segment::Trunk, btk::Segment::Central, "Trunk", "");
    model->AppendSegment(btk::Segment::Arm, btk::Segment::Right, "RArm", "");
    // Calibration
    btk::StaticCalibrationProcedure::Pointer scp = btk::StaticCalibrationProcedure::New();
    btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
    cal->SetAcquisition(acq);
    cal->SetModel(model);
    cal->SetSubject(subject);
    cal->SetCalibrationProcedure(scp);
    btk::Model::Pointer output = cal->GetOutput();
    output->Update();
    
    btk::Segment::Pointer seg;
    btk::Frame::Axis TFx, TFy, TFz, AFx, AFy, AFz;
    btk::Frame::Origin AFo, TFo;
    // Trunk
    TFo << -72.0653164384, -113.0161271918, 1097.4540036301;
    TFx << 0.9990208699, 0.0280923768, -0.0341777682;
    TFy << 0.0349023381, -0.0257198909, 0.9990597149;
    TFz << 0.0271869135, -0.9992743895, -0.0266751974;
    AFo << 86.2581302099, -21.0216470741, 0.0000000000;
    AFx << 0.9980485036, -0.0624434499, -0.0000000000;
    AFy << 0.0624434499, 0.9980485036, -0.0000000000;
    AFz << -0.0000000000, 0.0000000000, 1.0000000000;
    seg = output->GetSegment(0);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
    // Right Arm
    TFo << -68.5893408904, -407.0302015068, 1015.8376139041;
    TFx << 0.9399805099, -0.2335915663, 0.2487400674;
    TFy << 0.0337479841, 0.7890220092, 0.6134373175;
    TFz << -0.3395551716, -0.5682246467, 0.7495485551;
    AFo << 20.3242875484, 101.9281579640, -85.0997106446;
    AFx << 0.9973039536, -0.0660075007, 0.0320598491;
    AFy << 0.0726443022, 0.9498443546, -0.3041685510;
    AFz << -0.0103744608, 0.3056774639, 0.9520785990;
    seg = output->GetSegment(1);
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetOrigin().isApprox(TFo, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisX().isApprox(TFx, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisY().isApprox(TFy, 1e-8));
    TS_ASSERT(seg->GetMotion()->GetFrame(0).GetAxisZ().isApprox(TFz, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetOrigin().isApprox(AFo, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisX().isApprox(AFx, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisY().isApprox(AFy, 1e-8));
    TS_ASSERT(seg->GetRelativeAF().GetAxisZ().isApprox(AFz, 1e-8));
  }
};

CXXTEST_SUITE_REGISTRATION(ModelCalibrationFilterTest)
CXXTEST_TEST_REGISTRATION(ModelCalibrationFilterTest, NoInputs)
CXXTEST_TEST_REGISTRATION(ModelCalibrationFilterTest, StaticUpperLimbModelLeftMissingLandmark)
CXXTEST_TEST_REGISTRATION(ModelCalibrationFilterTest, StaticUpperLimbModelLeftNoTranslator)
CXXTEST_TEST_REGISTRATION(ModelCalibrationFilterTest, StaticUpperLimbModelBothWithTranslator)
CXXTEST_TEST_REGISTRATION(ModelCalibrationFilterTest, StaticUpperLimbModelBothLabelRenamedNoTranslator)
CXXTEST_TEST_REGISTRATION(ModelCalibrationFilterTest, StaticUpperLimbModelOnlyTwoSegments)
#endif