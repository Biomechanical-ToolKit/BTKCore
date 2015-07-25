#ifndef LZOTest_h
#define LZOTest_h

#include <btklzo.h>
#include <cstring> // memcmp

const unsigned char _lzo_test_encoded[40] = {0x00, 0x0D, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x74, 0x65, 0x78, 0x74, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x2E, 0x0A, 0x54, 0x68, 0x69, 0x73, 0x20, 0x7D, 0x68, 0x00, 0x11, 0x00, 0x00};
const char _lzo_test_decoded[] = "This is my test text file.\nThis is my test text file.\nThis is my test text file.\nThis is my test text file.\nThis is my test text file.\nThis is my test text file.\nThis is my test text file.\n";

CXXTEST_SUITE(LZOTest)
{
  CXXTEST_TEST(Decode)
  {
    unsigned char out[189] = {0};
    int inlen = sizeof(_lzo_test_encoded);
    int outlen = sizeof(out);
    int remlen = outlen;
    btk::lzo1x_decode(out, &remlen, _lzo_test_encoded, &inlen);
    TS_ASSERT_EQUALS(remlen, 0); // All the buffer was used
    TS_ASSERT_EQUALS(memcmp(out, _lzo_test_decoded, outlen), 0);
  };
};

CXXTEST_SUITE_REGISTRATION(LZOTest)
CXXTEST_TEST_REGISTRATION(LZOTest, Decode)

#endif // LZOTest_h