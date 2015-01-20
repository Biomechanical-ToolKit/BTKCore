#ifndef BinaryStreamTest_classdef_h
#define BinaryStreamTest_classdef_h

#include <btkIODevice.h>
#include <btkIODevice_p.h>
#include <cstring> // memcpy

class DummyBufferPrivate : public btk::IODevicePrivate
{
public:
  DummyBufferPrivate(char* d) : btk::IODevicePrivate(), Data(d), Pos(0) {};
  
  char* Data;
   btk::IODevice::Offset Pos;
};

class DummyBuffer : public btk::IODevice
{
  BTK_DECLARE_PIMPL_ACCESSOR(DummyBuffer)
  
public:
  DummyBuffer(char* d) : btk::IODevice(*new DummyBufferPrivate(d))
  {};
  virtual bool isOpen() const noexcept {return true;};
  virtual void close() {};
  virtual void read(char* s, Size n)
  {
    auto optr = this->pimpl();
    memcpy(s,optr->Data + optr->Pos, n);
    optr->Pos += n;
  };
  virtual void write(const char* s, Size n)
  {
    auto optr = this->pimpl();
    memcpy(optr->Data + optr->Pos, s, n);
    optr->Pos += n;
  };
  virtual void seek(Offset , SeekDir ) {}; // Not implemented
  virtual Position tell() const noexcept {return Position(Offset(-1));}; // Not implemented
  virtual bool isSequential() const noexcept {return true;};
  
  void setPos(Offset pos)
  {
    auto optr = this->pimpl();
    optr->Pos = pos;
  }
};

#endif // BinaryStreamTest_classdef_h