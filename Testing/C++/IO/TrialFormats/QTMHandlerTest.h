#ifndef QTMFileIOTest_h
#define QTMFileIOTest_h

#include <btkQTMHandler.h>
#include <btkFile.h>
#include <btkNode.h>
#include <btkTimeSequence.h>
#include <btkEvent.h>
#include <btkTrial.h>

CXXTEST_SUITE(QTMFileIOTest)
{
  CXXTEST_TEST(Capacity)
  {
    btk::QTMHandler format;
    TS_ASSERT_EQUALS(format.capability() == btk::QTMHandler::Capability::Read, true);
    TS_ASSERT_EQUALS((format.capability() & btk::QTMHandler::Capability::Read) == btk::QTMHandler::Capability::Read, true);
  };
  
  CXXTEST_TEST(SupportedTrialFormats)
  {
    btk::QTMHandler format;
    auto strs = format.supportedTrialFormats();
    TS_ASSERT_EQUALS(strs.size(), 1ul);
    TS_ASSERT_EQUALS(strs[0], std::string("QTM"));
  };
  
  CXXTEST_TEST(AvailableOptions)
  {
    const std::vector<std::string> ref_options = {"Encoding","ByteOrder","DataStorage"};
    const std::vector<std::string> ref_encoding_choices = {"Binary"};
    const std::vector<std::string> ref_byteorder_choices = {"IEEELittleEndian","IEEEBigEndian"};
    const std::vector<std::string> ref_datastorage_choices = {"Float"};
    btk::QTMHandler format;
    // Options
    auto options = format.availableOptions();
    TS_ASSERT_EQUALS(options.size(), 3ul);
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
  };
  
  CXXTEST_TEST(OptionsValue)
  {
    btk::QTMHandler format;
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::EncodingFormat>(), btk::QTMHandler::Encoding::Binary);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::ByteOrderFormat>(), btk::QTMHandler::ByteOrder::IEEEBigEndian);
#else
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::ByteOrderFormat>(), btk::QTMHandler::ByteOrder::IEEELittleEndian);
#endif
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::DataStorageFormat>(), btk::QTMHandler::DataStorage::Float);
    format.setOption<btk::QTMHandler::EncodingFormat>(btk::QTMHandler::Encoding::Binary);
    format.setOption<btk::QTMHandler::ByteOrderFormat>(btk::QTMHandler::ByteOrder::IEEELittleEndian);
    format.setOption<btk::QTMHandler::DataStorageFormat>(btk::QTMHandler::DataStorage::Float);
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::EncodingFormat>(), btk::QTMHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::ByteOrderFormat>(), btk::QTMHandler::ByteOrder::IEEELittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::DataStorageFormat>(), btk::QTMHandler::DataStorage::Float);
  };
  
  CXXTEST_TEST(DetectSignatureNoDevice)
  {
    btk::QTMHandler format;
    TS_ASSERT_EQUALS(format.detectSignature(), btk::QTMHandler::Signature::Invalid);
  };
  
  CXXTEST_TEST(DetectSignatureDeviceNotOpen)
  {
    btk::File file;
    btk::QTMHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::QTMHandler::Signature::Invalid);
  };

  CXXTEST_TEST(DetectSignatureEmptyFile)
  {
    btk::File file; file.open(_BTK_TDD_QTM_IN_"Empty.qtm", btk::File::Mode::In);
    btk::QTMHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::QTMHandler::Signature::Invalid);
  };

  CXXTEST_TEST(DetectSignatureFail)
  {
    btk::File file; file.open(_BTK_TDD_QTM_IN_"Fail.qtm", btk::File::Mode::In);
    btk::QTMHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::QTMHandler::Signature::Invalid);
  };

  CXXTEST_TEST(DetectSignatureOk)
  {
    btk::File file; file.open(_BTK_TDD_QTM_IN_"Static LB 1.qtm", btk::File::Mode::In);
    btk::QTMHandler format; format.setDevice(&file);
    TS_ASSERT_EQUALS(format.detectSignature(), btk::QTMHandler::Signature::Valid);
  };
  
  CXXTEST_TEST(StaticLB1)
  {
    btk::File file; file.open(_BTK_TDD_QTM_IN_"Static LB 1.qtm", btk::File::Mode::In);
    btk::QTMHandler format; format.setDevice(&file);
    btk::Node root("root");
    
    TS_ASSERT_EQUALS(format.read(&root),true);
    TS_ASSERT_EQUALS(format.errorCode(), btk::IOHandler::Error::None);
    TS_ASSERT_EQUALS(format.errorMessage(), "");
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::EncodingFormat>(), btk::QTMHandler::Encoding::Binary);
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::ByteOrderFormat>(), btk::QTMHandler::ByteOrder::IEEELittleEndian);
    TS_ASSERT_EQUALS(format.option<btk::QTMHandler::DataStorageFormat>(), btk::QTMHandler::DataStorage::Float);
    TS_ASSERT_EQUALS(root.children().size(),1ul);
  };
};

CXXTEST_SUITE_REGISTRATION(QTMFileIOTest)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, Capacity)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, SupportedTrialFormats)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, AvailableOptions)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, OptionsValue)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, DetectSignatureNoDevice)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, DetectSignatureDeviceNotOpen)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, DetectSignatureEmptyFile)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, DetectSignatureFail)
// CXXTEST_TEST_REGISTRATION(QTMFileIOTest, DetectSignatureOk)
CXXTEST_TEST_REGISTRATION(QTMFileIOTest, StaticLB1)
  
#endif