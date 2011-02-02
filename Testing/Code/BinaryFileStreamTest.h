#ifndef BinaryFileStream_h
#define BinaryFileStream_h

#include <btkBinaryFileStream.h>
#include <cstdio>

CXXTEST_SUITE(BinaryFileStreamTest)
{
  CXXTEST_TEST(DefaultConstructor)
  {
    btk::NativeBinaryFileStream bfs;
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    TS_ASSERT_EQUALS(bfs.GetExceptions(), std::ios_base::goodbit);
  };
  
  CXXTEST_TEST(SetExceptions)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.SetExceptions(btk::BinaryFileStream::EndFileBit | btk::BinaryFileStream::FailBit | btk::BinaryFileStream::BadBit);
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    TS_ASSERT_EQUALS(bfs.GetExceptions(), btk::BinaryFileStream::EndFileBit | btk::BinaryFileStream::FailBit | btk::BinaryFileStream::BadBit);
  };
  
  CXXTEST_TEST(CloseNoFile)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
  };
  
  CXXTEST_TEST(CloseNoFileException)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.SetExceptions(btk::BinaryFileStream::EndFileBit | btk::BinaryFileStream::FailBit | btk::BinaryFileStream::BadBit);
    TS_ASSERT_THROWS(bfs.Close(), btk::BinaryFileStreamException);
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
  };
  
  CXXTEST_TEST(SecondConstructorRead)
  {
    btk::NativeBinaryFileStream bfs(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(SecondConstructorWrongFilenameRead)
  {
    btk::NativeBinaryFileStream bfs("Wrong.test", btk::BinaryFileStream::In);
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
  };
  
  CXXTEST_TEST(SecondConstructorWriteNewFile)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::NativeBinaryFileStream bfs(filename, btk::BinaryFileStream::Out);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(SecondConstructorWriteExistedFile)
  {
    btk::NativeBinaryFileStream bfs(C3DFilePathOUT + "mmfstream.c3d", btk::BinaryFileStream::Out);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(OpenReadMode)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(OpenWriteMode)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::NativeBinaryFileStream bfs;
    bfs.Open(filename, btk::BinaryFileStream::Out);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(OpenWriteModeFromExistingFile)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathOUT + "mmfstream.c3d", btk::BinaryFileStream::Out);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(Read)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x02);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x50);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(ReadNoFile)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.ReadI8();
    bfs.ReadI8();
    bfs.ReadI8();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
  };
  
  CXXTEST_TEST(SeekReadBegin)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(0, btk::BinaryFileStream::Begin);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x02);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x50);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(SeekReadEnd)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(0, btk::BinaryFileStream::End);
    TS_ASSERT(bfs.TellRead() == std::streampos(406528));
    bfs.ReadI8();
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Clear();
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.SeekRead(-2, btk::BinaryFileStream::End);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x15);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x00);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(SeekReadBeginInvalid)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(-1, btk::BinaryFileStream::Begin);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Clear();
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x02);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x50);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
  };
  
  CXXTEST_TEST(SeekReadEndInvalid)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(1, btk::BinaryFileStream::End);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Clear();
    bfs.ReadI8();
    bfs.ReadI8();
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
  };
  
  CXXTEST_TEST(SeekReadEndInvalidBis)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(-(406528 + 1), btk::BinaryFileStream::End);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Clear();
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x02);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x50);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
  };
  
  CXXTEST_TEST(SeekReadCurrentInvalidForward)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(406528 + 1, btk::BinaryFileStream::Current);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Clear();
    bfs.ReadI8();
    bfs.ReadI8();
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
  };
  
  CXXTEST_TEST(SeekReadCurrentInvalidBackward)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(-1, btk::BinaryFileStream::Current);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Clear();
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x02);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x50);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
  };
  
  CXXTEST_TEST(SeekReadCurrentInvalidBackwardBis)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(0, btk::BinaryFileStream::End);
    bfs.SeekRead(-(406528 + 1), btk::BinaryFileStream::Current);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Clear();
    bfs.ReadI8();
    bfs.ReadI8();
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), false);
    TS_ASSERT_EQUALS(bfs.EndFile(), true);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), true);
    bfs.Close();
  };
  
  CXXTEST_TEST(ReadEOFException)
  {
    btk::NativeBinaryFileStream bfs;
    bfs.SetExceptions(btk::BinaryFileStream::EndFileBit | btk::BinaryFileStream::FailBit | btk::BinaryFileStream::BadBit);
    bfs.Open(C3DFilePathIN + "others/Gait.c3d", btk::BinaryFileStream::In);
    bfs.SeekRead(406526, btk::BinaryFileStream::Begin);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x15);
    TS_ASSERT_EQUALS(bfs.ReadI8(), 0x00);
    TS_ASSERT_THROWS(bfs.ReadI8(), btk::BinaryFileStreamException);
  };
  
  CXXTEST_TEST(Write)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::NativeBinaryFileStream bfs;
    bfs.Open(filename, btk::BinaryFileStream::Out);
    bfs.Write((int8_t)16);
    bfs.Close();
  };
  
  CXXTEST_TEST(SeekWrite)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::NativeBinaryFileStream bfs;
    bfs.Open(filename, btk::BinaryFileStream::Out);
#ifdef _MSC_VER // The granularity is not the same
    bfs.SeekWrite(65536, btk::BinaryFileStream::Begin);
#else
    bfs.SeekWrite(4096, btk::BinaryFileStream::Begin);
#endif
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Write((int8_t)16);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
  
  CXXTEST_TEST(SuperSeekWrite)
  {
    std::string filename = C3DFilePathOUT + "mmfstream.c3d";
    std::remove(filename.c_str());
    btk::NativeBinaryFileStream bfs;
    bfs.Open(filename, btk::BinaryFileStream::Out);
    bfs.SeekWrite(400000, btk::BinaryFileStream::Begin);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Write((int8_t)16);
    TS_ASSERT_EQUALS(bfs.IsOpen(), true);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
    bfs.Close();
    TS_ASSERT_EQUALS(bfs.IsOpen(), false);
    TS_ASSERT_EQUALS(bfs.Good(), true);
    TS_ASSERT_EQUALS(bfs.EndFile(), false);
    TS_ASSERT_EQUALS(bfs.Bad(), false);
    TS_ASSERT_EQUALS(bfs.Fail(), false);
  };
};

CXXTEST_SUITE_REGISTRATION(BinaryFileStreamTest)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, DefaultConstructor)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SetExceptions)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, CloseNoFile)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, CloseNoFileException)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SecondConstructorRead)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SecondConstructorWrongFilenameRead)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SecondConstructorWriteNewFile)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SecondConstructorWriteExistedFile)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, OpenReadMode)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, OpenWriteMode)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, OpenWriteModeFromExistingFile)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, Read)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, ReadNoFile)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadBegin)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadEnd)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadBeginInvalid)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadEndInvalid)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadEndInvalidBis)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadCurrentInvalidForward)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadCurrentInvalidBackward)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekReadCurrentInvalidBackwardBis)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, ReadEOFException)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, Write)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SeekWrite)
CXXTEST_TEST_REGISTRATION(BinaryFileStreamTest, SuperSeekWrite)
#endif
