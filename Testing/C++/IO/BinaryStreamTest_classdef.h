#ifndef BinaryStreamTest_classdef_h
#define BinaryStreamTest_classdef_h

#include <btkIODevice.h>
#include <btkIODevice_p.h>
#include <btkMacros.h> // _BTK_NOEXCEPT
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
  virtual bool isOpen() const _BTK_NOEXCEPT  override {return true;};
  virtual void close()  override {};
  virtual Size peek(char* s, Size n) const override
  {
    auto optr = this->pimpl();
    memcpy(s,optr->Data + optr->Pos, n);
    return n;
  };
  virtual void read(char* s, Size n) override
  {
    auto optr = this->pimpl();
    optr->Pos += this->peek(s,n);
  };
  virtual void write(const char* s, Size n) override
  {
    auto optr = this->pimpl();
    memcpy(optr->Data + optr->Pos, s, n);
    optr->Pos += n;
  };
  virtual void seek(Offset , Origin ) override {}; // Not implemented
  virtual Position tell() const _BTK_NOEXCEPT  override {return Position(Offset(-1));}; // Not implemented
  virtual bool isSequential() const _BTK_NOEXCEPT  override {return true;};
  virtual const char* data() const _BTK_NOEXCEPT override {return nullptr;};
  virtual Size size() const _BTK_NOEXCEPT override {return 0;};
  
  void setPos(Offset pos)
  {
    auto optr = this->pimpl();
    optr->Pos = pos;
  }
};

#endif // BinaryStreamTest_classdef_h