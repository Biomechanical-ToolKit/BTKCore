#include <btkLogger.h>

struct Verifier : btk::Logger::Device
{
  Verifier() : btk::Logger::Device(), ref() {this->ref = {0};};
  ~Verifier() = default;
  
  virtual void writeMessage(btk::Logger::Category , const char* msg) noexcept
  {
    TSM_ASSERT_EQUALS(msg,strcmp(msg,this->ref),0);
  };
  
  const char* ref;
};