#ifndef LoggerTest_h
#define LoggerTest_h

#include "LoggerTest_classdef.h"

CXXTEST_SUITE(LoggerTest)
{
  CXXTEST_TEST(VariadicArguments)
  {
    Verifier* verifier = new Verifier;
    btk::Logger::setDevice(verifier); // deleted by the logger
    
    verifier->ref = "foo";
    btk::Logger::info("foo");
    btk::Logger::warning("foo");
    btk::Logger::error("foo");

    verifier->ref = "I have an integer 12345 included.";
    btk::Logger::info("I have an integer %i included.",12345);
    btk::Logger::warning("I have an integer %i included.",12345);
    btk::Logger::error("I have an integer %i included.",12345);

    verifier->ref = "tiny at the beginning but huge after the variadic expansion. 1.54";
    btk::Logger::info("tiny %s. %0.2f","at the beginning but huge after the variadic expansion",1.54f);
    btk::Logger::warning("tiny %s. %0.2f","at the beginning but huge after the variadic expansion",1.54f);
    btk::Logger::error("tiny %s. %0.2f","at the beginning but huge after the variadic expansion",1.54f);
  };
};

CXXTEST_SUITE_REGISTRATION(LoggerTest)
CXXTEST_TEST_REGISTRATION(LoggerTest, VariadicArguments)

#endif // LoggerTest_h