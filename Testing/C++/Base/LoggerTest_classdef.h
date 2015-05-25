#include <btkLogger.h>
#include <btkMacros.h> // _BTK_NOEXCEPT

struct Verifier : btk::Logger::Device
{
  Verifier() : btk::Logger::Device(), ref(nullptr) {};
  ~Verifier() = default;
  
  virtual void writeMessage(btk::Logger::Category , const char* msg) _BTK_NOEXCEPT
  {
    TSM_ASSERT_EQUALS(msg,strcmp(msg,this->ref),0);
  };
  
  const char* ref;
};