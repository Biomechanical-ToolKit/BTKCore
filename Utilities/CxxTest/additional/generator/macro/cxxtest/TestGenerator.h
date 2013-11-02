/* 
 * Additional part for CxxTest
 * Copyright (c) 2009-2013, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __cxxtest_test_generator_h__
#define __cxxtest_test_generator_h__

// ------------------------------------------------------------------------- //
//                               GENERATOR MACROS
// ------------------------------------------------------------------------- //

#define CXXTEST_SUITE(s) \
        CXXTEST_SYMBOL_FILE(s, __FILE__) \
        CXXTEST_SYMBOL_LINE(s, __LINE__) \
        struct s: public CxxTest::TestSuite

#define CXXTEST_SYMBOL_FILE(s, f) \
        static const char* s##_##file = f;

#define CXXTEST_SYMBOL_LINE(s, l) \
        static const int s##_##line = l;

#define CXXTEST_TEST(t) \
        CXXTEST_SYMBOL_LINE(t, __LINE__) \
	      void t(void)

#define CXXTEST_SUITE_REGISTRATION(s) \
        static s suite_##s; \
        static CxxTest::List Tests_##s = { 0, 0 }; \
        CxxTest::StaticSuiteDescription suiteDescription_##s(s##_##file, s##_##line, #s, suite_##s, Tests_##s);

#define CXXTEST_TEST_REGISTRATION(s, t) \
        static class TestDescription_##s##_##t: public  CxxTest::RealTestDescription \
        { \
        public: \
	TestDescription_##s##_##t(): CxxTest::RealTestDescription(Tests_##s, suiteDescription_##s, s::t##_##line , #t) {}; \
	void runTest() { suite_##s.t(); }; \
        } testDescription_##s##_##t;
        
#define CXXTEST_SUITE_SETUP \
        void setUp
          
#define CXXTEST_SUITE_TEARDOWN \
        void tearDown
                
// ------------------------------------------------------------------------- //
//                               CXXTEST SYMBOLS
// ------------------------------------------------------------------------- //
        
#ifndef CXXTEST_RUNNING
    #define CXXTEST_RUNNING
#endif

// The next two lines has to be activated manually by the user in the 'main.cpp'
// file (or any other file with the main() function) BEFORE to include the
// header 'cxxtestgen.h'
//#define _CXXTEST_HAVE_STD
//#define _CXXTEST_HAVE_EH

// ------------------------------------------------------------------------- //
//                           REQUIRED CXXTEST HEADERS
// ------------------------------------------------------------------------- //

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
        
// You can modify the next line to include another printer (XML, XUnit, etc.)
#include <cxxtest/ErrorPrinter.h>

#endif // __cxxtest_test_generator_h__