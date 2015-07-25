#ifndef BufferTest_h
#define BufferTest_h

#include <btkBuffer.h>
#include <array>

template <size_t N>
std::array<char,N> int2char(std::array<int,N>&& data)
{
  std::array<char,N> arr;
  for (size_t i = 0 ; i < N ; ++i)
    arr[i] = char(data[i]);
  return arr;
}

CXXTEST_SUITE(BufferTest)
{
  CXXTEST_TEST(ContinousBuffer)
  {
    const auto array = int2char<41>({{0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04, 0x00, 0xAB, 0xAA, 0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72}});
    btk::Buffer buffer;
    buffer.open(array.data(), array.size());
    char test[20] = {0};
    buffer.read(test,20);
    const char* data = array.data();
    for (int i = 0 ; i < 20 ; ++i)
      TS_ASSERT_EQUALS(test[i],data[i+0]);
    buffer.read(test,20);
    for (int i = 0 ; i < 20 ; ++i)
      TS_ASSERT_EQUALS(test[i],data[i+20]);
    buffer.read(test,20); // Should not crash and read only one element
    for (int i = 0 ; i < 1 ; ++i)
      TS_ASSERT_EQUALS(test[i],data[i+40]);
    TS_ASSERT(!buffer.hasError());
    TS_ASSERT(buffer.hasFailure());
  };
  
  CXXTEST_TEST(ContinousBuffer_EndException)
  {
    auto array = int2char<41>({{0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04, 0x00, 0xAB, 0xAA, 0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72}});
    btk::Buffer buffer;
    buffer.setExceptions(btk::Buffer::State::End);
    buffer.open(array.data(), array.size(), btk::Buffer::Mode::In);
    char test[20] = {0};
    buffer.read(test,20);
    buffer.read(test,20);
    TS_ASSERT_THROWS_EQUALS(buffer.read(test,20), const btk::Buffer::Failure &f, f.what(), std::string("IODevice::clear"));
  };
  
  CXXTEST_TEST(ChunkBuffer)
  {
    auto array = int2char<50>(
      {{0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73,    // #2
        0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // #4
        0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04,    // #0 
        0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F,    // #3
        0x00, 0xAB, 0xAA, 0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F}}); // #1
    std::vector<size_t> chunkIds({2,4,0,3,1});
    btk::Buffer buffer;
    buffer.open(array.data(), array.size(), chunkIds, 10, btk::Buffer::Mode::In);
    TS_ASSERT_EQUALS(buffer.isOpen(), true);
    TS_ASSERT_EQUALS(buffer.isGood(), true);
    TS_ASSERT_EQUALS(buffer.atEnd(), false);
    auto ref = int2char<50>({{0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04, 0x00, 0xAB, 0xAA, 0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}});
    char test[50] = {0};
    TS_ASSERT_EQUALS(buffer.peek(test,50),50);
    for (int i = 0 ; i < 50 ; ++i)
      TS_ASSERT_EQUALS(test[i],ref[i]);
    TS_ASSERT(!buffer.hasError());
    TS_ASSERT(!buffer.hasFailure());
    TS_ASSERT(!buffer.atEnd());
    buffer.read(test,50);
    for (int i = 0 ; i < 50 ; ++i)
      TS_ASSERT_EQUALS(test[i],ref[i]);
    TS_ASSERT(!buffer.hasError());
    TS_ASSERT(!buffer.hasFailure());
    TS_ASSERT(!buffer.atEnd());
    buffer.read(test,1);
    TS_ASSERT(buffer.atEnd());
  };
  
  CXXTEST_TEST(ChunkBuffer_Seek)
  {
    auto array = int2char<50>(
      {{0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73,    // #2
        0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // #4
        0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04,    // #0 
        0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F,    // #3
        0x00, 0xAB, 0xAA, 0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F}}); // #1
    std::vector<size_t> chunkIds({2,4,0,3,1});
    btk::Buffer buffer;
    buffer.open(array.data(), array.size(), chunkIds, 10, btk::Buffer::Mode::In);
    auto ref = int2char<50>({{0xFB, 0x01, 0x53, 0x43, 0x41, 0x4C, 0x45, 0x22, 0x00, 0x04, 0x00, 0xAB, 0xAA, 0xAA, 0x3D, 0x19, 0x2A, 0x20, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x63, 0x61, 0x6C, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}});
    char test[10] = {0};
    buffer.seek(5, btk::Buffer::Origin::Begin);
    buffer.read(test,10);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(test[i],ref[i+5]);
    buffer.seek(0, btk::Buffer::Origin::Begin);
    buffer.read(test,10);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(test[i],ref[i]);
    buffer.seek(22, btk::Buffer::Origin::Current);
    buffer.read(test,10);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(test[i],ref[i+32]);
    buffer.seek(-10, btk::Buffer::Origin::End);
    buffer.read(test,10);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(test[i],ref[i+40]);
    TS_ASSERT(!buffer.hasError());
    TS_ASSERT(!buffer.hasFailure());
    TS_ASSERT(!buffer.atEnd());
  };
  
  CXXTEST_TEST(ChunkBuffer_Write)
  {
    char data[50] = {0};
    std::vector<size_t> chunkIds({2,4,0,3,1});
    btk::Buffer buffer;
    buffer.open(data, 50, chunkIds, 10, btk::Buffer::Mode::In); // Has to be Out (or In|Out) to write in the buffer.
    buffer.setExceptions(btk::Buffer::State::Fail);
    char c = '0';
    TS_ASSERT_THROWS_EQUALS(buffer.write(&c,1), const btk::Buffer::Failure &f, f.what(), std::string("IODevice::clear"));
    buffer.clear();
    buffer.close();
    buffer.open(data, 50, chunkIds, 10, btk::Buffer::Mode::Out);
    for (char i = 0 ; i < 50 ; ++i)
      buffer.write(&i,1);
    auto ref = int2char<50>(
      {{0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13}});
    for (int i = 0 ; i < 50 ; ++i)
      TS_ASSERT_EQUALS(data[i],ref[i]);
    TS_ASSERT(!buffer.hasError());
    TS_ASSERT(!buffer.hasFailure());
    TS_ASSERT(!buffer.atEnd());
  };
};

CXXTEST_SUITE_REGISTRATION(BufferTest)
CXXTEST_TEST_REGISTRATION(BufferTest, ContinousBuffer)
CXXTEST_TEST_REGISTRATION(BufferTest, ContinousBuffer_EndException)
CXXTEST_TEST_REGISTRATION(BufferTest, ChunkBuffer)
CXXTEST_TEST_REGISTRATION(BufferTest, ChunkBuffer_Seek)
CXXTEST_TEST_REGISTRATION(BufferTest, ChunkBuffer_Write)

#endif // BufferTest_h
