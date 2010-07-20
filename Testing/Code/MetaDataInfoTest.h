#ifndef MetaDataInfoTest_h
#define MetaDataInfoTest_h

#include <btkMetaDataInfo.h>
#include <btkConvert.h>

CXXTEST_SUITE(MetaDataInfoTest)
{
  CXXTEST_TEST(ConstructorInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), 5);
  };
  
  CXXTEST_TEST(ConstructorInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)5);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 5);
  };
  
  CXXTEST_TEST(ConstructorFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)5.0);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[0]), 5.0, 0.00001);
  };
  
  CXXTEST_TEST(ConstructorChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("test");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetDimensions()[0], 4);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "test");
  };
  
  CXXTEST_TEST(ConstructorVectorInt8)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 2);
    std::vector<int8_t> val = std::vector<int8_t>(4, 55);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    for (int i = 0 ; i < 4 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[i]), 55);
  };
  
  CXXTEST_TEST(ConstructorVectorInt16)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 3);
    std::vector<int16_t> val = std::vector<int16_t>(4, 655);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    for (int i = 0 ; i < 4 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[i]), 655);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[4]), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[5]), 0);
  };
  
  CXXTEST_TEST(ConstructorVectorFloat)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 3);
    std::vector<float> val = std::vector<float>(4, (float)273.45);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    for (int i = 0 ; i < 4 ; ++i)
      TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[i]), 273.45, 0.0001);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[4]), 0.0, 0.00001);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[5]), 0.0, 0.00001);    
  };
  
  CXXTEST_TEST(ConstructorVectorCharNormal)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(4, 4);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 4);
    for (int i = 0 ; i < 4 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[i]), "test");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeEmptyDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(0);
    std::vector<std::string> val = std::vector<std::string>(0);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), " ");

  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeLowerDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(0);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "t");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeUpperDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(1, 5);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "test ");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeEmptyDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2,5);
    std::vector<std::string> val = std::vector<std::string>(0);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[i]), "     ");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeLowerDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 2);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 2);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "te");
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[1]), "te");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeUpperDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 4 ; ++i)
    {
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[i]), "test ");
    }
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[4]), "     ");

  };
  
  CXXTEST_TEST(ConstructorVectorCharPointLabels)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 0); dim[0] = 32;
    std::vector<std::string> val = std::vector<std::string>(0);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 0);
  };
  
  CXXTEST_TEST(SetValueCharInChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, "FOOBAR");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "FOOBAR");
  };
  
  CXXTEST_TEST(SetValueCharInInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, "FOOBAR");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), 0);
  };
  
  CXXTEST_TEST(SetValueCharInInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)5);
    test->SetValue(0, "FOOBAR");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 0);
  };
  
  CXXTEST_TEST(SetValueCharInFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)5.1234);
    test->SetValue(0, "FOOBAR");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<float*>(test->GetValues()[0]), 0.0);
  };
  
  CXXTEST_TEST(SetValueCharInChar_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, "12345");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "12345");
  };
  
  CXXTEST_TEST(SetValueCharInInt8_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, "45");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), 45);
  };
  
  CXXTEST_TEST(SetValueCharInInt16_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)5);
    test->SetValue(0, "12345");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 12345);
  };
  
  CXXTEST_TEST(SetValueCharInFloat_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)5.1234);
    test->SetValue(0, "1.2345");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[0]), 1.2345, 1e-5);
  };
  
  CXXTEST_TEST(SetValueInt8InChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, (int8_t)15);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "15");
  };
  
  CXXTEST_TEST(SetValueInt8InChar_Uint8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, (int8_t)128);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "-128");
  };
  
  CXXTEST_TEST(SetValueInt8InInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, (int8_t)15);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), 15);
  };
  
  CXXTEST_TEST(SetValueInt8InInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)5);
    test->SetValue(0, (int8_t)15);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 15);
  };
  
  CXXTEST_TEST(SetValueInt8InFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)5.1234);
    test->SetValue(0, (int8_t)15);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<float*>(test->GetValues()[0]), 15.0);
  };
  
  CXXTEST_TEST(SetValueInt16InChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, (int16_t)1024);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "1024");
  };
  
  CXXTEST_TEST(SetValueInt16InInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, (int16_t)12456);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), (int8_t)168);
  };
  
  CXXTEST_TEST(SetValueInt16InInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)2048);
    test->SetValue(0, (int16_t)12456);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 12456);
  };
  
  CXXTEST_TEST(SetValueInt16InFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)2.456);
    test->SetValue(0, (int16_t)4000);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<float*>(test->GetValues()[0]), 4000.0);
  };
  
  CXXTEST_TEST(SetValueFloatInChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, (float)1.2345);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "1.2345");
  };
  
  CXXTEST_TEST(SetValueFloatInInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, (float)1.2345);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), (int8_t)1);
  };
  
  CXXTEST_TEST(SetValueFloatInInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)2048);
    test->SetValue(0, (float)1.2345);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 1);
  };
  
  CXXTEST_TEST(SetValueFloatInFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)2.456);
    test->SetValue(0, (float)3.14);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[0]), 3.14, 1e-5);
  };
  
  CXXTEST_TEST(SetValueIntInChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, 1234567);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "1234567");
  };
  
  CXXTEST_TEST(SetValueIntInInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, 1234567);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), (int8_t)135);
  };
  
  CXXTEST_TEST(SetValueIntInInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)2048);
    test->SetValue(0, 1234567);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), -10617);
  };
  
  CXXTEST_TEST(SetValueIntInFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)2.456);
    test->SetValue(0, 1234567);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[0]), 1234567.0, 1e-5);
  };
  
  CXXTEST_TEST(SetValueDoubleInChar)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("FOO");
    test->SetValue(0, 1.23456789);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValues()[0]), "1.23456789");
  };
  
  CXXTEST_TEST(SetValueDoubleInInt8)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    test->SetValue(0, 1.23456789);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Byte);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int8_t*>(test->GetValues()[0]), (int8_t)1);
  };
  
  CXXTEST_TEST(SetValueDoubleInInt16)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int16_t)2048);
    test->SetValue(0, 1.23456789);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValues()[0]), 1);
  };
  
  CXXTEST_TEST(SetValueDoubleInFloat)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)2.456);
    test->SetValue(0, 1.23456789);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValues()[0]), (float)1.23456, 1e-5);
  };

  CXXTEST_TEST(SetValuesFromVectorString)
  {
    std::vector<std::string> val = std::vector<std::string>(0);
    val.push_back("NAME");
    val.push_back("CALIBRATION");
    val.push_back("FULL_DESCRIPTION");
    val.push_back("SETUP");
    val.push_back("DATE");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetDimension(0), 16);
    TS_ASSERT_EQUALS(test->GetDimension(1), 5);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(0)), "NAME            ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(1)), "CALIBRATION     ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(2)), "FULL_DESCRIPTION");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(3)), "SETUP           ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(4)), "DATE            ");
  };

  CXXTEST_TEST(SetValueFromString)
  {
    std::vector<std::string> val = std::vector<std::string>(0);
    val.push_back("NAME");
    val.push_back("CALIBRATION");
    val.push_back("FULL_DESCRIPTION");
    val.push_back("SETUP");
    val.push_back("DATE");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(val);
    test->SetValue(2,"SHORTER");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetDimension(0), 16);
    TS_ASSERT_EQUALS(test->GetDimension(1), 5);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(0)), "NAME            ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(1)), "CALIBRATION     ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(2)), "SHORTER         ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(3)), "SETUP           ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(4)), "DATE            ");
    test->SetValue(2,"BIGGERANDBIGGERANDBIGGER");
    TS_ASSERT_EQUALS(test->GetDimension(0), 24);
    TS_ASSERT_EQUALS(test->GetDimension(1), 5);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(0)), "NAME                    ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(1)), "CALIBRATION             ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(2)), "BIGGERANDBIGGERANDBIGGER");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(3)), "SETUP                   ");
    TS_ASSERT_EQUALS(*static_cast<const std::string*>(test->GetValue(4)), "DATE                    ");
  };
  
  CXXTEST_TEST(SetFormatChar2Integer)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    test->SetFormat(btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValue(i)), 0);
  };
  
  CXXTEST_TEST(SetFormatFloat2Integer)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<float> val = std::vector<float>(25, (float)1.950);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    test->SetFormat(btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Integer);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 25);
    for (int i = 0 ; i < 25 ; ++i)
    {
      TS_ASSERT_EQUALS(*static_cast<int16_t*>(test->GetValue(i)), 1);
    }
  };
  
  CXXTEST_TEST(SetFormatFloat2CharDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(0);
    std::vector<float> val = std::vector<float>(1, (float)1.95);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    test->SetFormat(btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetDimensions().at(0), 4);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValue(0)), "1.95");
  };

  CXXTEST_TEST(SetFormatFloat2CharDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<float> val = std::vector<float>(25, (float)1.95);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    test->SetFormat(btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 3);
    TS_ASSERT_EQUALS(test->GetDimensions().at(0), 4);
    TS_ASSERT_EQUALS(test->GetDimensions().at(1), 5);
    TS_ASSERT_EQUALS(test->GetDimensions().at(2), 5);
    TS_ASSERT_EQUALS(test->GetValues().size(), 25);
    for (int i = 0 ; i < 25 ; ++i)
      TS_ASSERT_EQUALS(test->ToString(i), "1.95");
      //TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValue(i)), "1.95");
  };
  
  CXXTEST_TEST(SetDimensionForFloat)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<float> val = std::vector<float>(25, (float)1.950);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    test->SetDimension(0, 6);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 30);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<float*>(test->GetValue(i*6+5)), 0.0);
    test->SetDimension(1, 6);
    TS_ASSERT_EQUALS(test->GetValues().size(), 36);
    for (int i = 30 ; i < 35 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<float*>(test->GetValue(i)), 0.0);
  };
  
  CXXTEST_TEST(SetDimensionForChar)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<std::string> val = std::vector<std::string>(5, "test ");
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    test->SetDimension(0, 2);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValue(i)), "te");
  };
  
  CXXTEST_TEST(ResizeDimensionsFrom0To1Byte)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)0);
    test->ResizeDimensions(1);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
  };
  
  CXXTEST_TEST(ResizeDimensionsFrom3To1Float)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(3, 3);
    std::vector<float> val = std::vector<float>(27, (float)1.950);
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(dim, val);
    test->ResizeDimensions(1);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues().size(), 3);
  };
  
  CXXTEST_TEST(ResizeDimensionsFrom1To0Char)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New("test");
    test->ResizeDimensions(0);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test->GetValue(0)), "t");

  };
  
  CXXTEST_TEST(Clone)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)1.435);
    btk::MetaDataInfo::Pointer test2 = test->Clone();
    test2->SetValues("test");
    TS_ASSERT_DELTA(*static_cast<float*>(test->GetValue(0)),1.435, 0.0001);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(*static_cast<std::string*>(test2->GetValue(0)), "test");
    TS_ASSERT_EQUALS(test2->GetFormat(), btk::MetaDataInfo::Char);
  };
  
  CXXTEST_TEST(Equality)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((float)1.435);
    btk::MetaDataInfo::Pointer test2 = btk::MetaDataInfo::New((float)1.435);
    TS_ASSERT(*test == *test2);
    test2->SetValues("Allo");
    TS_ASSERT(*test != *test2);
    test2->SetValues((float)1.435);
    TS_ASSERT(*test == *test2);
    test2->SetValues((float)2.5);
    TS_ASSERT(*test != *test2);
    test->SetValues(std::vector<int8_t>(5,5));
    test2->SetValues(std::vector<int8_t>(5,5));
    TS_ASSERT(*test == *test2);
    test2->SetValue(0, (int8_t)15);
    TS_ASSERT(*test != *test2);
  };

  CXXTEST_TEST(String2String)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "test"));
    std::vector<std::string> val = test->ToString();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), "test");
  };

  CXXTEST_TEST(Byte2String)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    std::vector<std::string> val = test->ToString();
    TS_ASSERT_EQUALS(val.size(), 1);
    TS_ASSERT_EQUALS(val.at(0), "5");
  };

  CXXTEST_TEST(Integer2String)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<int16_t>(5, 379));
    std::vector<std::string> val = test->ToString();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), "379");
  };

  CXXTEST_TEST(Real2String)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<float>(5, 0.83333f));
    std::vector<std::string> val = test->ToString();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), "0.83333");
  };

  CXXTEST_TEST(String2Byte)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "test"));
    std::vector<int8_t> val = test->ToInt8();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 0);
  };

  CXXTEST_TEST(Byte2Byte)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    std::vector<int8_t> val = test->ToInt8();
    TS_ASSERT_EQUALS(val.size(), 1);
    TS_ASSERT_EQUALS(val.at(0), 5);
  };

  CXXTEST_TEST(Integer2Byte)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<int16_t>(5, 379));
    std::vector<int8_t> val = test->ToInt8();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 123);
  };

  CXXTEST_TEST(Real2Byte)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<float>(5, 0.1f));
    std::vector<int8_t> val = test->ToInt8();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 0);
  };

  CXXTEST_TEST(String2Integer)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "test"));
    std::vector<int16_t> val = test->ToInt16();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 0);
  };

  CXXTEST_TEST(Byte2Integer)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    std::vector<int16_t> val = test->ToInt16();
    TS_ASSERT_EQUALS(val.size(), 1);
    TS_ASSERT_EQUALS(val.at(0), 5);
  };

  CXXTEST_TEST(Integer2Integer)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<int16_t>(5, 379));
    std::vector<int16_t> val = test->ToInt16();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 379);
  };

  CXXTEST_TEST(Real2Integer)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<float>(5, 0.1f));
    std::vector<int16_t> val = test->ToInt16();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 0);
  };

  CXXTEST_TEST(String2Real)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "test"));
    std::vector<float> val = test->ToFloat();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(val.at(i), 0, 0.0001);
  };

  CXXTEST_TEST(Byte2Real)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New((int8_t)5);
    std::vector<float> val = test->ToFloat();
    TS_ASSERT_EQUALS(val.size(), 1);
    TS_ASSERT_DELTA(val.at(0), 5, 0.0001);
  };

  CXXTEST_TEST(Integer2Real)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<int16_t>(5, 379));
    std::vector<float> val = test->ToFloat();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(val.at(i), 379, 0.0001);
  };

  CXXTEST_TEST(Real2Real)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<float>(5, 0.1f));
    std::vector<float> val = test->ToFloat();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(val.at(i), 0.1, 0.0001);
  };

  CXXTEST_TEST(Float2Double)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<float>(5, 0.1f));
    std::vector<double> val = test->ToDouble();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(val.at(i), 0.1, 0.0001);
  };

  CXXTEST_TEST(Int82Int)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<int8_t>(5, 45));
    std::vector<int> val = test->ToInt();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 45);
  };

  CXXTEST_TEST(Int162Double)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<int16_t>(5, 369));
    std::vector<double> val = test->ToDouble();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 369);
  };

  CXXTEST_TEST(String2String_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "12345"));
    std::vector<std::string> val = test->ToString();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), "12345");
  };
  
  CXXTEST_TEST(String2Byte_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "45"));
    std::vector<int8_t> val = test->ToInt8();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), (int8_t)45);
  };
  
  CXXTEST_TEST(String2Integer_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "12345"));
    std::vector<int16_t> val = test->ToInt16();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), 12345);
  };
  
  CXXTEST_TEST(String2Real_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<std::string>(5, "1.2345"));
    std::vector<float> val = test->ToFloat();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(val.at(i), 1.2345, 0.0001);
  };
  
  CXXTEST_TEST(Real2String2Real_Number)
  {
    btk::MetaDataInfo::Pointer test = btk::MetaDataInfo::New(std::vector<float>(5, 1.2345));
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetDimensions().at(0), 5);
    test->SetFormat(btk::MetaDataInfo::Char);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetDimensions().at(0), 6);
    TS_ASSERT_EQUALS(test->GetDimensions().at(1), 5);
    std::vector<std::string> val = test->ToString();
    TS_ASSERT_EQUALS(val.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(val.at(i), "1.2345");
    
    test->SetFormat(btk::MetaDataInfo::Real);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetDimensions().at(0), 5);
    std::vector<float> val2 = test->ToFloat();
    TS_ASSERT_EQUALS(val2.size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(val2.at(i), 1.2345, 0.0001);
  };
};

CXXTEST_SUITE_REGISTRATION(MetaDataInfoTest)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharNormal)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharResizeEmptyDim1)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharResizeLowerDim1)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharResizeUpperDim1)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharResizeEmptyDim2)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharResizeLowerDim2)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharResizeUpperDim2)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ConstructorVectorCharPointLabels)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInChar_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInInt8_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInInt16_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueCharInFloat_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt8InChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt8InChar_Uint8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt8InInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt8InInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt8InFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt16InChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt16InInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt16InInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueInt16InFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueFloatInChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueFloatInInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueFloatInInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueFloatInFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueIntInChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueIntInInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueIntInInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueIntInFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueDoubleInChar)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueDoubleInInt8)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueDoubleInInt16)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueDoubleInFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValuesFromVectorString)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetValueFromString)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetFormatChar2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetFormatFloat2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetFormatFloat2CharDim1)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetFormatFloat2CharDim2)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, SetDimensionForFloat)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ResizeDimensionsFrom0To1Byte)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ResizeDimensionsFrom3To1Float)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, ResizeDimensionsFrom1To0Char)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Clone)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Equality)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2String)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Byte2String)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Integer2String)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Real2String)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2Byte)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Byte2Byte)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Integer2Byte)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Real2Byte)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Byte2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Integer2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Real2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2Real)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Byte2Real)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Integer2Real)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Real2Real)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Float2Double)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Int82Int)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Int162Double)  
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2String_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2Byte_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2Integer_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, String2Real_Number)
CXXTEST_TEST_REGISTRATION(MetaDataInfoTest, Real2String2Real_Number)

#endif
