#ifndef FileTest_h
#define FileTest_h

#include <btkFile.h>
#include <cstdio>

CXXTEST_SUITE(FileTest)
{
  CXXTEST_TEST(DefaultConstructor)
  {
    btk::File file;
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    TS_ASSERT_EQUALS(file.exceptions(), btk::File::GoodBit);
  };
  
  CXXTEST_TEST(SetExceptions)
  {
    btk::File file;
    file.setExceptions(btk::File::EndBit | btk::File::FailBit | btk::File::ErrorBit);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    TS_ASSERT_EQUALS(file.exceptions(), btk::File::EndBit | btk::File::FailBit | btk::File::ErrorBit);
  };
  
  CXXTEST_TEST(CloseNoFile)
  {
    btk::File file;
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(CloseNoFileException)
  {
    btk::File file;
    file.setExceptions(btk::File::EndBit | btk::File::FailBit | btk::File::ErrorBit);
    TS_ASSERT_THROWS(file.close(), btk::File::Failure);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SecondConstructorRead)
  {
    btk::File file(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(SecondConstructorWrongFilenameRead)
  {
    btk::File file("Wrong.test", btk::File::In);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SecondConstructorWriteNewFile)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::File file(filename, btk::File::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(SecondConstructorWriteExistedFile)
  {
    btk::File file(C3DFilePathOUT + "mmfstream.c3d", btk::File::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(OpenReadMode)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(OpenWriteMode)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::File file;
    file.open(filename, btk::File::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(OpenWriteModeFromExistingFile)
  {
    btk::File file;
    file.open(C3DFilePathOUT + "mmfstream.c3d", btk::File::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(Read)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(ReadNoFile)
  {
    btk::File file;
    char buf[3] = {0};
    file.read(buf,3);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SeekBegin)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(0, btk::File::Begin);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(SeekEnd)
  {
    btk::File file;
    char buf[1] = {0};
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(0, btk::File::End);
    TS_ASSERT(file.tell() == btk::File::Position(406528));
    file.read(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.seek(-2, btk::File::End);
    file.read(buf,1);
    TS_ASSERT_EQUALS(buf[0], 0x15);
    file.read(buf,1);
    TS_ASSERT_EQUALS(buf[0], 0x00);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(SeekBeginInvalid)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(-1, btk::File::Begin);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(SeekEndInvalid)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(1, btk::File::End);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SeekEndInvalidBis)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(-(406528 + 1), btk::File::End);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(SeekCurrentInvalidForward)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(406528 + 1, btk::File::Current);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SeekCurrentInvalidBackward)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(-1, btk::File::Current);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(SeekCurrentInvalidBackwardBis)
  {
    btk::File file;
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(0, btk::File::End);
    file.seek(-(406528 + 1), btk::File::Current);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.isEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.close();
  };
  
  CXXTEST_TEST(ReadEOFException)
  {
    btk::File file;
    file.setExceptions(btk::File::EndBit | btk::File::FailBit | btk::File::ErrorBit);
    file.open(C3DFilePathIN + "others/Gait.c3d", btk::File::In);
    file.seek(406526, btk::File::Begin);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x15);
    TS_ASSERT_EQUALS(buf[1], 0x00);
    TS_ASSERT_THROWS(file.read(buf,1), btk::File::Failure);
  };
  
  CXXTEST_TEST(Write)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::File file;
    file.open(filename, btk::File::Out);
    char buf[1] = {0x16};
    file.write(buf,1);
    file.close();
  };
  
  CXXTEST_TEST(SeekWrite)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::File file;
    file.open(filename, btk::File::Out);
#ifdef _MSC_VER // The granularity is not the same
    file.seek(65536, btk::File::Begin);
#else
    file.seek(4096, btk::File::Begin);
#endif
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    char buf[1] = {0x16};
    file.write(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(SuperSeekWrite)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::File file;
    file.open(filename, btk::File::Out);
    file.seek(400000, btk::File::Begin);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    char buf[1] = {0x16};
    file.write(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.isEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
};

CXXTEST_SUITE_REGISTRATION(FileTest)
CXXTEST_TEST_REGISTRATION(FileTest, DefaultConstructor)
CXXTEST_TEST_REGISTRATION(FileTest, SetExceptions)
CXXTEST_TEST_REGISTRATION(FileTest, CloseNoFile)
CXXTEST_TEST_REGISTRATION(FileTest, CloseNoFileException)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorRead)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorWrongFilenameRead)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorWriteNewFile)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorWriteExistedFile)
CXXTEST_TEST_REGISTRATION(FileTest, OpenReadMode)
CXXTEST_TEST_REGISTRATION(FileTest, OpenWriteMode)
CXXTEST_TEST_REGISTRATION(FileTest, OpenWriteModeFromExistingFile)
CXXTEST_TEST_REGISTRATION(FileTest, Read)
CXXTEST_TEST_REGISTRATION(FileTest, ReadNoFile)
CXXTEST_TEST_REGISTRATION(FileTest, SeekBegin)
CXXTEST_TEST_REGISTRATION(FileTest, SeekEnd)
CXXTEST_TEST_REGISTRATION(FileTest, SeekBeginInvalid)
CXXTEST_TEST_REGISTRATION(FileTest, SeekEndInvalid)
CXXTEST_TEST_REGISTRATION(FileTest, SeekEndInvalidBis)
CXXTEST_TEST_REGISTRATION(FileTest, SeekCurrentInvalidForward)
CXXTEST_TEST_REGISTRATION(FileTest, SeekCurrentInvalidBackward)
CXXTEST_TEST_REGISTRATION(FileTest, SeekCurrentInvalidBackwardBis)
CXXTEST_TEST_REGISTRATION(FileTest, ReadEOFException)
CXXTEST_TEST_REGISTRATION(FileTest, Write)
CXXTEST_TEST_REGISTRATION(FileTest, SeekWrite)
CXXTEST_TEST_REGISTRATION(FileTest, SuperSeekWrite)

#endif // FileTest_h
