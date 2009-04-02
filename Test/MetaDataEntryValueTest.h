#ifndef MetaDataEntryValueTest_h
#define MetaDataEntryValueTest_h

#include <btkMetaDataEntryValue.h>
#include <btkConvert.h>

CXXTEST_SUITE(MetaDataEntryValueTest)
{
  CXXTEST_TEST(ConstructorInt8)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New((int8_t)5);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::BYTE);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues()[0], "\x05");
  };
  
  CXXTEST_TEST(ConstructorInt16)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New((int16_t)5);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::INTEGER);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues()[0], "5");
  };
  
  CXXTEST_TEST(ConstructorFloat)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New((float)5.0);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::FLOAT);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues()[0], "5");
  };
  
  CXXTEST_TEST(ConstructorChar)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New("test");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetDimensions()[0], 4);
    TS_ASSERT_EQUALS(test->GetValues()[0], "test");
  };
  
  CXXTEST_TEST(ConstructorVectorInt8)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 2);
    std::vector<int8_t> val = std::vector<int8_t>(4, 55);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::BYTE);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    for (int i = 0 ; i < 4 ; ++i)
    {
      int8_t val = 0;
      btk::FromString(test->GetValues()[i], val);
      TS_ASSERT_EQUALS(val, 55);
    }
  };
  
  CXXTEST_TEST(ConstructorVectorInt16)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 3);
    std::vector<int16_t> val = std::vector<int16_t>(4, 655);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::INTEGER);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    for (int i = 0 ; i < 4 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValues()[i], "655");
    }
    TS_ASSERT_EQUALS(test->GetValues()[4], "0");
    TS_ASSERT_EQUALS(test->GetValues()[5], "0");
  };
  
  CXXTEST_TEST(ConstructorVectorFloat)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 3);
    std::vector<float> val = std::vector<float>(4, (float)273.45);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::FLOAT);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    for (int i = 0 ; i < 4 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValues()[i], "273.45");
    }
    TS_ASSERT_EQUALS(test->GetValues()[4], "0");
    TS_ASSERT_EQUALS(test->GetValues()[5], "0");
  };
  
  CXXTEST_TEST(ConstructorVectorCharNormal)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(4, 4);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 4);
    for (int i = 0 ; i < 4 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValues()[i], "test");
    }
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeEmptyDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(0);
    std::vector<std::string> val = std::vector<std::string>(0);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues()[0], " ");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeLowerDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(0);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues()[0], "t");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeUpperDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(1, 5);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues()[0], "test ");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeEmptyDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2,5);
    std::vector<std::string> val = std::vector<std::string>(0);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValues()[i], "     ");
    }
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeLowerDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 2);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues()[0], "te");
    TS_ASSERT_EQUALS(test->GetValues()[1], "te");
  };
  
  CXXTEST_TEST(ConstructorVectorCharResizeUpperDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 4 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValues()[i], "test ");
    }
    TS_ASSERT_EQUALS(test->GetValues()[4], "     ");
  };
  
  CXXTEST_TEST(ConstructorVectorCharPointLabels)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 0); dim[0] = 32;
    std::vector<std::string> val = std::vector<std::string>(0);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 0);
  };
  
  CXXTEST_TEST(SetFormatChar2Integer)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<std::string> val = std::vector<std::string>(4, "test");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    test->SetFormat(btk::MetaDataEntryValue::INTEGER);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::INTEGER);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 25);
    for (int i = 0 ; i < 25 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValue(i), "0");
    }
  };
  
  CXXTEST_TEST(SetFormatFloat2Integer)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<float> val = std::vector<float>(25, (float)1.950);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::FLOAT);
    test->SetFormat(btk::MetaDataEntryValue::INTEGER);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::INTEGER);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 25);
    for (int i = 0 ; i < 25 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValue(i), "0");
    }
  };
  
  CXXTEST_TEST(SetFormatFloat2CharDim1)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(0);
    std::vector<float> val = std::vector<float>(1, (float)1.950);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::FLOAT);
    test->SetFormat(btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(test->GetValue(0), " ");
  };

  CXXTEST_TEST(SetFormatFloat2CharDim2)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<float> val = std::vector<float>(25, (float)1.950);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::FLOAT);
    test->SetFormat(btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::CHAR);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValue(i), "     ");
    }
  };
  
  CXXTEST_TEST(SetDimensionForFloat)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<float> val = std::vector<float>(25, (float)1.950);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    test->SetDimension(0, 6);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 30);
    for (int i = 0 ; i < 5 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValue(i*6+5), "0");
    };
    test->SetDimension(1, 6);
    TS_ASSERT_EQUALS(test->GetValues().size(), 36);
    for (int i = 30 ; i < 35 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValue(i), "0");
    }
  };
  
  CXXTEST_TEST(SetDimensionForChar)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 5);
    std::vector<std::string> val = std::vector<std::string>(5, "test ");
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    test->SetDimension(0, 2);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(test->GetValues().size(), 5);
    for (int i = 0 ; i < 5 ; ++i)
    {
      TS_ASSERT_EQUALS(test->GetValue(i), "te");
    }
  };
  
  CXXTEST_TEST(ResizeDimensionsFrom0To1Byte)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New((int8_t)0);
    test->ResizeDimensions(1);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
  };
  
  CXXTEST_TEST(ResizeDimensionsFrom3To1Float)
  {
    std::vector<uint8_t> dim = std::vector<uint8_t>(3, 3);
    std::vector<float> val = std::vector<float>(27, (float)1.950);
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New(dim, val);
    test->ResizeDimensions(1);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS(test->GetValues().size(), 3);
  };
  
  CXXTEST_TEST(ResizeDimensionsFrom1To0Char)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New("test");
    test->ResizeDimensions(0);
    TS_ASSERT_EQUALS(test->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS(test->GetValues().size(), 1);
    TS_ASSERT_EQUALS(test->GetValue(0), "t");
  };
  
  CXXTEST_TEST(Clone)
  {
    btk::MetaDataEntryValue::Pointer test = btk::MetaDataEntryValue::New((float)1.435);
    btk::MetaDataEntryValue::Pointer test2 = test->Clone();
    test2->SetValues("test");
    TS_ASSERT_EQUALS(test->GetValue(0), "1.435");
    TS_ASSERT_EQUALS(test->GetFormat(), btk::MetaDataEntryValue::FLOAT);
    TS_ASSERT_EQUALS(test2->GetValue(0), "test");
    TS_ASSERT_EQUALS(test2->GetFormat(), btk::MetaDataEntryValue::CHAR);
  };
};

CXXTEST_SUITE_REGISTRATION(MetaDataEntryValueTest)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorInt8)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorInt16)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorFloat)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorChar)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorInt8)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorInt16)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorFloat)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharNormal)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharResizeEmptyDim1)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharResizeLowerDim1)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharResizeUpperDim1)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharResizeEmptyDim2)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharResizeLowerDim2)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharResizeUpperDim2)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ConstructorVectorCharPointLabels)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, SetFormatChar2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, SetFormatFloat2Integer)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, SetFormatFloat2CharDim1)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, SetFormatFloat2CharDim2)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, SetDimensionForFloat)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ResizeDimensionsFrom0To1Byte)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ResizeDimensionsFrom3To1Float)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, ResizeDimensionsFrom1To0Char)
CXXTEST_TEST_REGISTRATION(MetaDataEntryValueTest, Clone)

#endif