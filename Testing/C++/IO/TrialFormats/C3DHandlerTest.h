#ifndef C3DFileIOTest_h
#define C3DFileIOTest_h

#include <btkC3DHandler.h>
#include <btkFile.h>
#include <btkNode.h>
#include <btkTimeSequence.h>
#include <btkEvent.h>
#include <btkTrial.h>

CXXTEST_SUITE(C3DFileIOTest)
{
  CXXTEST_TEST(Capacity)
  {
    btk::C3DHandler format;
    TS_ASSERT_EQUALS(format.capability() == btk::C3DHandler::Capability::ReadWrite, true);
    TS_ASSERT_EQUALS((format.capability() & btk::C3DHandler::Capability::Read) == btk::C3DHandler::Capability::Read, true);
    TS_ASSERT_EQUALS((format.capability() & btk::C3DHandler::Capability::Write) == btk::C3DHandler::Capability::Write, true);
  };
  
  CXXTEST_TEST(SupportedTrialFormats)
  {
    btk::C3DHandler format;
    auto strs = format.supportedTrialFormats();
    TS_ASSERT_EQUALS(strs.size(), 1ul);
    TS_ASSERT_EQUALS(strs[0], std::string("C3D"));
  };
  
  CXXTEST_TEST(AvailableOptions)
  {
    const std::vector<std::string> ref_options = {"Encoding","ByteOrder","DataStorage","AnalogStorage"};
    const std::vector<std::string> ref_encoding_choices = {"Binary"};
    const std::vector<std::string> ref_byteorder_choices = {"VAXLittleEndian","IEEELittleEndian","IEEEBigEndian"};
    const std::vector<std::string> ref_datastorage_choices = {"Integer","Float"};
    const std::vector<std::string> ref_analogstorage_choices = {"Signed","Unsigned"};
    btk::C3DHandler format;
    // Options
    auto options = format.availableOptions();
    TS_ASSERT_EQUALS(options.size(), 4ul);
    for (size_t i = 0 ; i < options.size() ; ++i)
      TS_ASSERT_EQUALS(options[i],ref_options[i]);
    std::vector<const char*> choices;
    // Encoding choices
    choices = format.availableOptionChoices(ref_options[0].c_str());
    TS_ASSERT_EQUALS(choices.size(), ref_encoding_choices.size());
    for (size_t i = 0 ; i < choices.size() ; ++i)
      TS_ASSERT_EQUALS(choices[i],ref_encoding_choices[i]);
    // ByteOrder choices
    choices = format.availableOptionChoices(ref_options[1].c_str());
    TS_ASSERT_EQUALS(choices.size(), ref_byteorder_choices.size());
    for (size_t i = 0 ; i < choices.size() ; ++i)
      TS_ASSERT_EQUALS(choices[i],ref_byteorder_choices[i]);
    // DataStorage choices
    choices = format.availableOptionChoices(ref_options[2].c_str());
    TS_ASSERT_EQUALS(choices.size(), ref_datastorage_choices.size());
    for (size_t i = 0 ; i < choices.size() ; ++i)
      TS_ASSERT_EQUALS(choices[i],ref_datastorage_choices[i]);
    // AnalogStage choices
    choices = format.availableOptionChoices(ref_options[3].c_str());
    TS_ASSERT_EQUALS(choices.size(), ref_analogstorage_choices.size());
    for (size_t i = 0 ; i < choices.size() ; ++i)
      TS_ASSERT_EQUALS(choices[i],ref_analogstorage_choices[i]);
  };
  
  CXXTEST_TEST(OptionsValue)
  {
    btk::C3DHandler format;
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::Native);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Float);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    format.setOption<btk::C3DHandler::EncodingFormat>(btk::C3DHandler::Encoding::Text); // Impossible as only the encoding format is available
    format.setOption<btk::C3DHandler::ByteOrderFormat>(btk::C3DHandler::ByteOrder::VAXLittleEndian);
    format.setOption<btk::C3DHandler::DataStorageFormat>(btk::C3DHandler::DataStorage::Integer);
    format.setOption<btk::C3DHandler::AnalogStorageFormat>(btk::C3DHandler::AnalogStorage::Unsigned);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::VAXLittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Integer);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Unsigned);
  };
  
  CXXTEST_TEST(DetectSignatureNoDevice)
  {
    btk::C3DHandler format;
    TS_ASSERT_EQUALS(format.detectSignature(), btk::C3DHandler::Signature::Invalid);
  };
  
  CXXTEST_TEST(DetectSignatureDeviceNotOpen)
  {
    btk::File file;
    btk::C3DHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::C3DHandler::Signature::Invalid);
  };

  CXXTEST_TEST(DetectSignatureEmptyFile)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"others/Empty.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::C3DHandler::Signature::Invalid);
  };

  CXXTEST_TEST(DetectSignatureFail)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"others/Fail.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::C3DHandler::Signature::Invalid);
  };

  CXXTEST_TEST(DetectSignatureOk)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015pi.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::C3DHandler::Signature::Valid);
  };
  
  CXXTEST_TEST(Sample01_Eb015pi)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015pi.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::IEEELittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Integer);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
    
    btk::Trial* trial = root.findChild<btk::Trial*>({},{},false);
    TS_ASSERT(trial != nullptr);
    
    btk::TimeSequence* ts = trial->timeSequences()->findChild<btk::TimeSequence*>("RFT1");
    TS_ASSERT_DIFFERS(ts, nullptr);
    TS_ASSERT_EQUALS(ts->description(), "DIST/LAT FOOT");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_DELTA(ts->data(8,0), 250.4166, 1e-4);
    TS_ASSERT_DELTA(ts->data(0,3), 1.3333, 1e-4);
    
    auto tss = trial->timeSequences()->findChildren<btk::TimeSequence*>();
    TS_ASSERT_EQUALS(tss.size(), 26u+16u); // 26 videos and 16 analogs
    auto itts = tss.cbegin();
    std::advance(itts,14); // Point #15
    TS_ASSERT_DELTA((*itts)->data(20,3), 1.91667, 1e-4);
    std::advance(itts,3); // Point #18
    TS_ASSERT_DELTA((*itts)->data(16,0), 285.0, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,1), 222.4166, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,2), 564.4166, 1e-4);
    itts = tss.cbegin();
    std::advance(itts,26); // Analog #1
    TS_ASSERT_DELTA((*itts)->data(0), -26.6599, 1e-3);
    TS_ASSERT_DELTA((*itts)->data(1), -25.8, 1e-4);
    std::advance(itts,2); // Analog #3
    TS_ASSERT_EQUALS((*itts)->description(), "FORCE Z-COMP 1");
    std::advance(itts,13); // Analog #16
    TS_ASSERT_EQUALS((*itts)->name(), "CH16");

    auto evts = trial->events()->findChildren<btk::Event*>();
    TS_ASSERT_EQUALS(evts.size(), 3u);
    auto itevt = evts.cbegin();
    TS_ASSERT_EQUALS((*itevt)->name(), "RIC");
    TS_ASSERT_DELTA((*itevt)->time(), 2.72, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RHS");
    TS_ASSERT_DELTA((*itevt)->time(), 5.40, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RTO");
    TS_ASSERT_DELTA((*itevt)->time(), 7.32, 1e-4);
  };
  
  CXXTEST_TEST(Sample01_Eb015si)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015si.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::IEEEBigEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Integer);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
    
    btk::Trial* trial = root.findChild<btk::Trial*>({},{},false);
    TS_ASSERT(trial != nullptr);
    
    btk::TimeSequence* ts = trial->timeSequences()->findChild<btk::TimeSequence*>("RFT1");
    TS_ASSERT_DIFFERS(ts, nullptr);
    TS_ASSERT_EQUALS(ts->description(), "DIST/LAT FOOT");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_DELTA(ts->data(8,0), 250.4166, 1e-4);
    TS_ASSERT_DELTA(ts->data(0,3), 1.3333, 1e-4);
    
    auto tss = trial->timeSequences()->findChildren<btk::TimeSequence*>();
    TS_ASSERT_EQUALS(tss.size(), 26u+16u); // 26 videos and 16 analogs
    auto itts = tss.cbegin();
    std::advance(itts,14); // Point #15
    TS_ASSERT_DELTA((*itts)->data(20,3), 1.91667, 1e-4);
    std::advance(itts,3); // Point #18
    TS_ASSERT_DELTA((*itts)->data(16,0), 285.0, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,1), 222.4166, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,2), 564.4166, 1e-4);
    itts = tss.cbegin();
    std::advance(itts,26); // Analog #1
    TS_ASSERT_DELTA((*itts)->data(0), -26.6599, 1e-3);
    TS_ASSERT_DELTA((*itts)->data(1), -25.8, 1e-4);
    std::advance(itts,2); // Analog #3
    TS_ASSERT_EQUALS((*itts)->description(), "FORCE Z-COMP 1");
    std::advance(itts,13); // Analog #16
    TS_ASSERT_EQUALS((*itts)->name(), "CH16");

    auto evts = trial->events()->findChildren<btk::Event*>();
    TS_ASSERT_EQUALS(evts.size(), 3u);
    auto itevt = evts.cbegin();
    TS_ASSERT_EQUALS((*itevt)->name(), "RIC");
    TS_ASSERT_DELTA((*itevt)->time(), 2.72, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RHS");
    TS_ASSERT_DELTA((*itevt)->time(), 5.40, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RTO");
    TS_ASSERT_DELTA((*itevt)->time(), 7.32, 1e-4);
  };
  
  CXXTEST_TEST(Sample01_Eb015vi)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015vi.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::VAXLittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Integer);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
    
    btk::Trial* trial = root.findChild<btk::Trial*>({},{},false);
    TS_ASSERT(trial != nullptr);
    
    btk::TimeSequence* ts = trial->timeSequences()->findChild<btk::TimeSequence*>("RFT1");
    TS_ASSERT_DIFFERS(ts, nullptr);
    TS_ASSERT_EQUALS(ts->description(), "DIST/LAT FOOT");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_DELTA(ts->data(8,0), 250.4166, 1e-4);
    TS_ASSERT_DELTA(ts->data(0,3), 1.3333, 1e-4);
    
    auto tss = trial->timeSequences()->findChildren<btk::TimeSequence*>();
    TS_ASSERT_EQUALS(tss.size(), 26u+16u); // 26 videos and 16 analogs
    auto itts = tss.cbegin();
    std::advance(itts,14); // Point #15
    TS_ASSERT_DELTA((*itts)->data(20,3), 1.91667, 1e-4);
    std::advance(itts,3); // Point #18
    TS_ASSERT_DELTA((*itts)->data(16,0), 285.0, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,1), 222.4166, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,2), 564.4166, 1e-4);
    itts = tss.cbegin();
    std::advance(itts,26); // Analog #1
    TS_ASSERT_DELTA((*itts)->data(0), -26.6599, 1e-3);
    TS_ASSERT_DELTA((*itts)->data(1), -25.8, 1e-4);
    std::advance(itts,2); // Analog #3
    TS_ASSERT_EQUALS((*itts)->description(), "FORCE Z-COMP 1");
    std::advance(itts,13); // Analog #16
    TS_ASSERT_EQUALS((*itts)->name(), "CH16");

    auto evts = trial->events()->findChildren<btk::Event*>();
    TS_ASSERT_EQUALS(evts.size(), 3u);
    auto itevt = evts.cbegin();
    TS_ASSERT_EQUALS((*itevt)->name(), "RIC");
    TS_ASSERT_DELTA((*itevt)->time(), 2.72, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RHS");
    TS_ASSERT_DELTA((*itevt)->time(), 5.40, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RTO");
    TS_ASSERT_DELTA((*itevt)->time(), 7.32, 1e-4);
  };
  
  CXXTEST_TEST(Sample01_Eb015pr)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015pr.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::IEEELittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Float);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
    
    btk::Trial* trial = root.findChild<btk::Trial*>({},{},false);
    TS_ASSERT(trial != nullptr);
    
    btk::TimeSequence* ts = trial->timeSequences()->findChild<btk::TimeSequence*>("RFT1");
    TS_ASSERT_DIFFERS(ts, nullptr);
    TS_ASSERT_EQUALS(ts->description(), "DIST/LAT FOOT");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_DELTA(ts->data(8,0), 250.4166, 1e-4);
    TS_ASSERT_DELTA(ts->data(0,3), 1.3333, 1e-4);
    
    auto tss = trial->timeSequences()->findChildren<btk::TimeSequence*>();
    TS_ASSERT_EQUALS(tss.size(), 26u+16u); // 26 videos and 16 analogs
    auto itts = tss.cbegin();
    std::advance(itts,14); // Point #15
    TS_ASSERT_DELTA((*itts)->data(20,3), 1.91667, 1e-4);
    std::advance(itts,3); // Point #18
    TS_ASSERT_DELTA((*itts)->data(16,0), 285.0, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,1), 222.4166, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,2), 564.4166, 1e-4);
    itts = tss.cbegin();
    std::advance(itts,26); // Analog #1
    TS_ASSERT_EQUALS((*itts)->sampleRate(), 200.0);
    TS_ASSERT_DELTA((*itts)->data(0), -26.6599, 1e-3);
    TS_ASSERT_DELTA((*itts)->data(1), -25.8, 1e-4);
    std::advance(itts,2); // Analog #3
    TS_ASSERT_EQUALS((*itts)->description(), "FORCE Z-COMP 1");
    std::advance(itts,13); // Analog #16
    TS_ASSERT_EQUALS((*itts)->name(), "CH16");

    auto evts = trial->events()->findChildren<btk::Event*>();
    TS_ASSERT_EQUALS(evts.size(), 3u);
    auto itevt = evts.cbegin();
    TS_ASSERT_EQUALS((*itevt)->name(), "RIC");
    TS_ASSERT_DELTA((*itevt)->time(), 2.72, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RHS");
    TS_ASSERT_DELTA((*itevt)->time(), 5.40, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RTO");
    TS_ASSERT_DELTA((*itevt)->time(), 7.32, 1e-4);
  };
  
  CXXTEST_TEST(Sample01_Eb015sr)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015sr.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::IEEEBigEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Float);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
    
    btk::Trial* trial = root.findChild<btk::Trial*>({},{},false);
    TS_ASSERT(trial != nullptr);
    
    btk::TimeSequence* ts = trial->timeSequences()->findChild<btk::TimeSequence*>("RFT1");
    TS_ASSERT_DIFFERS(ts, nullptr);
    TS_ASSERT_EQUALS(ts->description(), "DIST/LAT FOOT");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_DELTA(ts->data(8,0), 250.4166, 1e-4);
    TS_ASSERT_DELTA(ts->data(0,3), 1.3333, 1e-4);
    
    auto tss = trial->timeSequences()->findChildren<btk::TimeSequence*>();
    TS_ASSERT_EQUALS(tss.size(), 26u+16u); // 26 videos and 16 analogs
    auto itts = tss.cbegin();
    std::advance(itts,14); // Point #15
    TS_ASSERT_DELTA((*itts)->data(20,3), 1.91667, 1e-4);
    std::advance(itts,3); // Point #18
    TS_ASSERT_DELTA((*itts)->data(16,0), 285.0, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,1), 222.4166, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,2), 564.4166, 1e-4);
    itts = tss.cbegin();
    std::advance(itts,26); // Analog #1
    TS_ASSERT_DELTA((*itts)->data(0), -26.6599, 1e-3);
    TS_ASSERT_DELTA((*itts)->data(1), -25.8, 1e-4);
    std::advance(itts,2); // Analog #3
    TS_ASSERT_EQUALS((*itts)->description(), "FORCE Z-COMP 1");
    std::advance(itts,13); // Analog #16
    TS_ASSERT_EQUALS((*itts)->name(), "CH16");

    auto evts = trial->events()->findChildren<btk::Event*>();
    TS_ASSERT_EQUALS(evts.size(), 3u);
    auto itevt = evts.cbegin();
    TS_ASSERT_EQUALS((*itevt)->name(), "RIC");
    TS_ASSERT_DELTA((*itevt)->time(), 2.72, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RHS");
    TS_ASSERT_DELTA((*itevt)->time(), 5.40, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RTO");
    TS_ASSERT_DELTA((*itevt)->time(), 7.32, 1e-4);
  };
  
  CXXTEST_TEST(Sample01_Eb015vr)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample01/Eb015vr.c3d", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::EncodingFormat>(), btk::C3DHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::ByteOrderFormat>(), btk::C3DHandler::ByteOrder::VAXLittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::DataStorageFormat>(), btk::C3DHandler::DataStorage::Float);
    TS_ASSERT_EQUALS(format.option<btk::C3DHandler::AnalogStorageFormat>(), btk::C3DHandler::AnalogStorage::Signed);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
    
    btk::Trial* trial = root.findChild<btk::Trial*>({},{},false);
    TS_ASSERT(trial != nullptr);
    
    btk::TimeSequence* ts = trial->timeSequences()->findChild<btk::TimeSequence*>("RFT1");
    TS_ASSERT_DIFFERS(ts, nullptr);
    TS_ASSERT_EQUALS(ts->description(), "DIST/LAT FOOT");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_DELTA(ts->data(8,0), 250.4166, 1e-4);
    TS_ASSERT_DELTA(ts->data(0,3), 1.3333, 1e-4);
    
    auto tss = trial->timeSequences()->findChildren<btk::TimeSequence*>();
    TS_ASSERT_EQUALS(tss.size(), 26u+16u); // 26 videos and 16 analogs
    auto itts = tss.cbegin();
    std::advance(itts,14); // Point #15
    TS_ASSERT_DELTA((*itts)->data(20,3), 1.91667, 1e-4);
    std::advance(itts,3); // Point #18
    TS_ASSERT_DELTA((*itts)->data(16,0), 285.0, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,1), 222.4166, 1e-4);
    TS_ASSERT_DELTA((*itts)->data(16,2), 564.4166, 1e-4);
    itts = tss.cbegin();
    std::advance(itts,26); // Analog #1
    TS_ASSERT_DELTA((*itts)->data(0), -26.6599, 1e-3);
    TS_ASSERT_DELTA((*itts)->data(1), -25.8, 1e-4);
    std::advance(itts,2); // Analog #3
    TS_ASSERT_EQUALS((*itts)->description(), "FORCE Z-COMP 1");
    std::advance(itts,13); // Analog #16
    TS_ASSERT_EQUALS((*itts)->name(), "CH16");

    auto evts = trial->events()->findChildren<btk::Event*>();
    TS_ASSERT_EQUALS(evts.size(), 3u);
    auto itevt = evts.cbegin();
    TS_ASSERT_EQUALS((*itevt)->name(), "RIC");
    TS_ASSERT_DELTA((*itevt)->time(), 2.72, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RHS");
    TS_ASSERT_DELTA((*itevt)->time(), 5.40, 1e-4);
    ++itevt;
    TS_ASSERT_EQUALS((*itevt)->name(), "RTO");
    TS_ASSERT_DELTA((*itevt)->time(), 7.32, 1e-4);
  };

  CXXTEST_TEST(Sample02_DEC_INT)
  {
    btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample02/DEC_INT.C3D", btk::File::Mode::In);
    btk::C3DHandler format; format.setDevice(&file);
    btk::Node root("root");
    TS_ASSERT(format.read(&root));
    btk::Trial* trial = root.findChild<btk::Trial*>();
    TS_ASSERT(trial != nullptr);    
    TS_ASSERT_EQUALS(trial->timeSequences()->children().size(),36u+16u);
    btk::TimeSequence* ts = nullptr;
    ts = trial->timeSequence(3);
    TS_ASSERT_EQUALS(ts->name(), "RSK1");
    TS_ASSERT_EQUALS(ts->sampleRate(), 50.0);
    TS_ASSERT_EQUALS(ts->samples(), 89u);
    TS_ASSERT_DELTA(ts->data(16,0), 384.0944, 1e-4);
    TS_ASSERT_DELTA(ts->data(16,1), 523.8418, 1e-4);
    TS_ASSERT_DELTA(ts->data(16,2), 311.2683, 1e-4);
    ts = trial->timeSequence(36+2);
    TS_ASSERT_EQUALS(ts->name(), "FZ1");
    TS_ASSERT_EQUALS(ts->sampleRate(), 200.0);
    TS_ASSERT_EQUALS(ts->samples(), 89u*4u);
    TS_ASSERT_DELTA(ts->data(15), 8.9280, 1e-4);
    TS_ASSERT_DELTA(ts->data(39), 5.952, 1e-4);
  };
//
//   CXXTEST_TEST(Sample02_Dec_real)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample02/Dec_real.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);
//
//     TS_ASSERT_EQUALS(acq->GetPoint(3)->name(), "RSK1");
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8417, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 1e-4);
//
//     TS_ASSERT_EQUALS(acq->GetAnalog(2)->name(), "FZ1");
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 1e-4);
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 1e-4);
//   };
//
//   CXXTEST_TEST(Sample02_pc_int)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample02/pc_int.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);
//
//     TS_ASSERT_EQUALS(acq->GetPoint(3)->name(), "RSK1");
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.5606, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 1e-4);
//
//     TS_ASSERT_EQUALS(acq->GetAnalog(2)->name(), "FZ1");
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 1e-4);
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 1e-4);
//   };
//
//   CXXTEST_TEST(Sample02_pc_real)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample02/pc_real.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);
//
//     TS_ASSERT_EQUALS(acq->GetPoint(3)->name(), "RSK1");
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8417, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 1e-4);
//
//     TS_ASSERT_EQUALS(acq->GetAnalog(2)->name(), "FZ1");
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 1e-4);
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 1e-4);
//   };
//
//   CXXTEST_TEST(Sample02_sgi_int)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample02/sgi_int.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);
//
//     TS_ASSERT_EQUALS(acq->GetPoint(3)->name(), "RSK1");
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.5606, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 1e-4);
//
//     TS_ASSERT_EQUALS(acq->GetAnalog(2)->name(), "FZ1");
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 1e-4);
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 1e-4);
//   };
//
//   CXXTEST_TEST(Sample02_sgi_real)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample02/sgi_real.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 36);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 89 * 4);
//
//     TS_ASSERT_EQUALS(acq->GetPoint(3)->name(), "RSK1");
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,0), 384.0944, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,1), 523.8417, 1e-4);
//     TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(16,2), 311.2683, 1e-4);
//
//     TS_ASSERT_EQUALS(acq->GetAnalog(2)->name(), "FZ1");
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(15,0), 8.9280, 1e-4);
//     TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(39,0), 5.952, 1e-4);
//   };
//
//   CXXTEST_TEST(Sample05_vicon512)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample05/vicon512.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 120);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 45);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 6492);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1080);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 23);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 6492 * 9);
//   };
//
//   CXXTEST_TEST(Sample06_MACsample)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample06/MACsample.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//   };
//
//
//   CXXTEST_TEST(Sample08_TEST)
//   {
//     btk::File file; btk::C3DHandler format; btk::Node root("root");
//     // TESTAPI
//     file.open(_BTK_TDD_C3D_IN_"c3d.org/sample08/TESTAPI.c3d", btk::File::Mode::In);
//     format.setDevice(&file);
//     format.read(&root);
//     // TESTBPI
//     file.open(_BTK_TDD_C3D_IN_"c3d.org/sample08/TESTBPI.c3d", btk::File::Mode::In);
//     format.setDevice(&file);
//     format.read(&root);
//     // TESTCPI
//     file.open(_BTK_TDD_C3D_IN_"c3d.org/sample08/TESTCPI.c3d", btk::File::Mode::In);
//     format.setDevice(&file);
//     format.read(&root);
//     // TESTDPI
//     file.open(_BTK_TDD_C3D_IN_"c3d.org/sample08/TESTDPI.c3d", btk::File::Mode::In);
//     format.setDevice(&file);
//     format.read(&root);
//   };
//
//   CXXTEST_TEST(Sample09_PlugInC3D)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample09/PlugInC3D.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//     TS_ASSERT_EQUALS(acq->GetPoint(0)->GetType(), btk::Point::Marker);
//     TS_ASSERT_EQUALS(acq->GetPoint(18)->GetType(), btk::Point::Angle);
//     TS_ASSERT_EQUALS(acq->GetPoint(64)->GetType(), btk::Point::Force);
//     TS_ASSERT_EQUALS(acq->GetPoint(72)->GetType(), btk::Point::Moment);
//     TS_ASSERT_EQUALS(acq->GetPoint(80)->GetType(), btk::Point::Power);
//
//     TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(8)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(9)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(10)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(11)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(12)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(13)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(14)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(16)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(17)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(18)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(19)->GetGain(), btk::Analog::PlusMinus10);
//     TS_ASSERT_EQUALS(acq->GetAnalog(20)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(21)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(22)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(23)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(24)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(25)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(26)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(27)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(28)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(29)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(30)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(31)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(32)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(33)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(34)->GetGain(), btk::Analog::PlusMinus5);
//     TS_ASSERT_EQUALS(acq->GetAnalog(35)->GetGain(), btk::Analog::PlusMinus5);
//
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq->GetMetaData()->GetChild("POINT")->GetChild("FRAMES")->GetInfo()->ToUInt16(0));
//     btk::MetaDataInfo::Pointer actualFieldVal = acq->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo();
//     int frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
//     TS_ASSERT_EQUALS(acq->GetFirstFrame(), frameIndex);
//     actualFieldVal = acq->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo();
//     frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
//     TS_ASSERT_EQUALS(acq->GetLastFrame(), frameIndex);
//   }
//
//   CXXTEST_TEST(Sample13_Dance)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample13/Dance.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_DELTA(acq->GetPointFrequency(), 65.05, 0.01);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 40);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 499);
//     TS_ASSERT_DELTA(acq->GetAnalogFrequency(), 65.05, 0.01);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 499);
//   };
//
//   CXXTEST_TEST(Sample13_golfswing)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample13/golfswing.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_DELTA(acq->GetPointFrequency(), 107.53, 0.01);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 29);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 514);
//     TS_ASSERT_DELTA(acq->GetAnalogFrequency(), 107.53, 0.01);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 514);
//   };
//
//   CXXTEST_TEST(Sample15_FP1)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample15/FP1.C3D", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//   };
//
//   CXXTEST_TEST(Sample15_FP2)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample15/FP2.C3D", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//   };
//
//   CXXTEST_TEST(Sample18_bad_parameter_section)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample18/bad_parameter_section.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(!format.read(&root));
//     TS_ASSERT_EQUALS(format.errorCode(),btk::IOHandler::Error::InvalidData);
//     TS_ASSERT_EQUALS(format.errorMessage(), std::string("C3D - Bad data first block"));
//   };
//
//   CXXTEST_TEST(Sample19_sample19)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample19/sample19.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 60);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 34672);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1080);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 2);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 18 * 34672);
//   };
//
//   CXXTEST_TEST(Sample20_phasespace_sample)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample20/phasespace_sample.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 30);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 40);
//     TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 701);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 30);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 701);
//   };
//
//   CXXTEST_TEST(Sample21_sample21)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample21/sample21.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//   };
//
//   CXXTEST_TEST(Sample22_BKINtechnologies)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample22/BKINtechnologies.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetEventNumber(), 2);
//     TS_ASSERT_EQUALS(acq->GetEvent(0)->name(), "STAY_CENTRE");
//     TS_ASSERT_DELTA(acq->GetEvent(0)->time(), 0.0, 1e-4);
//     TS_ASSERT_EQUALS(acq->GetEvent(0)->GetDescription(), "subject must wait at centre starting now");
//     TS_ASSERT_EQUALS(acq->GetEvent(1)->name(), "TARGET_ON");
//     TS_ASSERT_DELTA(acq->GetEvent(1)->time(), 1.6140, 1e-4);
//     TS_ASSERT_EQUALS(acq->GetEvent(1)->GetDescription(), "Target light ON, start waiting for subject to leave centre target");
//   };
//
//   CXXTEST_TEST(Sample28_dynamic)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample28/dynamic.C3D", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//   };
//
//   CXXTEST_TEST(Sample28_standing)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample28/standing.C3D", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//   };
//
//   CXXTEST_TEST(Sample28_type1)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"c3d.org/sample28/type1.C3D", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//   };
//
//   CXXTEST_TEST(ParameterOverflow)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"others/parameterOverflow.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//     btk::MetaData::ConstIterator itAnalysis = acq->GetMetaData()->FindChild("ANALYSIS");
//     TS_ASSERT(itAnalysis != acq->GetMetaData()->End());
//     if (itAnalysis != acq->GetMetaData()->End())
//       TS_ASSERT((*itAnalysis)->FindChild("VALUES") != (*itAnalysis)->End());
//   };
//
//   CXXTEST_TEST(Mocap36)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"others/lard36.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//   };
//
//   CXXTEST_TEST(BadParameterOffset)
//   {
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"others/badParameterOffset.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 41);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
//     TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
//     TS_ASSERT_EQUALS(acq->GetLastFrame(), 111);
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 250.0);
//     TS_ASSERT_EQUALS(acq->GetPoint(0)->name(), "Point    1");
//     TS_ASSERT_EQUALS(acq->GetPoint(40)->name(), "Point   41");
//     TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,0), 168.093002, 1e-5);
//     TS_ASSERT_DELTA(acq->GetPoint(29)->GetValues().coeff(10,1), 731.903015, 1e-5);
//     TS_ASSERT_DELTA(acq->GetPoint(40)->GetValues().coeff(110,2), 0.0, 1e-5);
//   };
//
//   CXXTEST_TEST(UTF8)
//   {
//     // The Windows charset seems not compatible with hardcoded UTF-8 strings. The test is discarded under Windows.
// #if !defined(_WIN32)
//     btk::File file; file.open(_BTK_TDD_C3D_IN_"others/Я могу есть стекло/оно мне не вредит.c3d", btk::File::Mode::In);
//     btk::C3DHandler format; format.setDevice(&file);
//     btk::Node root("root");
//     TS_ASSERT(format.read(&root));
//
//     TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
//     TS_ASSERT_EQUALS(acq->GetPointNumber(), 27);
//     TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 6);
//     TS_ASSERT_EQUALS(acq->GetFirstFrame(), 91);
//     TS_ASSERT_EQUALS(acq->GetLastFrame(), 327);
//     TS_ASSERT_EQUALS(acq->GetPointFrequency(), 250.0);
//     TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
//     TS_ASSERT_EQUALS(acq->GetPoint(0)->name(), "AbcdeFghijk:RASI");
//     TS_ASSERT_EQUALS(acq->GetPoint(26)->name(), "AbcdeFghijk:LFIN");
// #endif
//   };
};

CXXTEST_SUITE_REGISTRATION(C3DFileIOTest)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Capacity)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, SupportedTrialFormats)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, AvailableOptions)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, OptionsValue)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, DetectSignatureNoDevice)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, DetectSignatureDeviceNotOpen)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, DetectSignatureEmptyFile)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, DetectSignatureFail)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, DetectSignatureOk)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample01_Eb015pi)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample01_Eb015si)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample01_Eb015vi)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample01_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample01_Eb015sr)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample01_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample02_DEC_INT)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample02_Dec_real)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample02_pc_int)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample02_pc_real)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample02_sgi_int)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample02_sgi_real)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample05_vicon512)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample06_MACsample)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample08_TEST)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample09_PlugInC3D)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample13_Dance)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample13_golfswing)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample15_FP1)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample15_FP2)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample18_bad_parameter_section)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample19_sample19)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample20_phasespace_sample)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample21_sample21)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample22_BKINtechnologies)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample28_dynamic)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample28_standing)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Sample28_type1)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, ParameterOverflow)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, Mocap36)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, BadParameterOffset)
// CXXTEST_TEST_REGISTRATION(C3DFileIOTest, UTF8)
#endif
