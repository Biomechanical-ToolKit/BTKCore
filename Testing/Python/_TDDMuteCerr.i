%module MuteCerr
%{
#define SWIG_FILE_WITH_INIT
#include <iostream>
static void On();
static void Off();
static std::streambuf* MuteCerrOldBuf = 0;
%}

%inline
%{
void On()
{
  MuteCerrOldBuf = std::cerr.rdbuf(0);
};

void Off()
{
  if (MuteCerrOldBuf != 0)
    std::cerr.rdbuf(MuteCerrOldBuf);
};  
%}
