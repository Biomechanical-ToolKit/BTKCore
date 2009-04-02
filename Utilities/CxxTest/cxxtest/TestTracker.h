#ifndef __cxxtest__TestTracker_h__
#define __cxxtest__TestTracker_h__

//
// The TestTracker tracks running tests
// The actual work is done in CountingListenerProxy,
// but this way avoids cyclic references TestListener<->CountingListenerProxy
//

#include <cxxtest/TestListener.h>
#include <cxxtest/DummyDescriptions.h>

namespace CxxTest
{
    class TestListener;
    
    class TestTracker : public TestListener
    {
    public:
        virtual ~TestTracker();
        
        static TestTracker &tracker();

        const TestDescription *fixTest( const TestDescription *d ) const;
        const SuiteDescription *fixSuite( const SuiteDescription *d ) const;
        const WorldDescription *fixWorld( const WorldDescription *d ) const;

        const TestDescription &test() const { return *_test; }
        const SuiteDescription &suite() const { return *_suite; }
        const WorldDescription &world() const { return *_world; }
        
		bool testWarned() const { return (testWarnedAsserts() > 0); }
        bool suiteWarned() const { return (suiteWarnedTests() > 0); }
        bool worldWarned() const { return (warnedSuites() > 0); }
		
        bool testFailed() const { return (testFailedAsserts() > 0); }
        bool suiteFailed() const { return (suiteFailedTests() > 0); }
        bool worldFailed() const { return (failedSuites() > 0); }
		
		bool testUnexpected() const { return (testUnexpectedAsserts() > 0); }
        bool suiteUnexpected() const { return (suiteUnexpectedTests() > 0); }
        bool worldUnexpected() const { return (unexpectedSuites() > 0); }
        
		unsigned warnedTests() const { return _warnedTests; }
		unsigned testWarnedAsserts() const { return _testWarnedAsserts; }
		unsigned suiteWarnedTests() const { return _suiteWarnedTests; }
		unsigned warnedSuites() const { return _warnedSuites; }
		
        unsigned failedTests() const { return _failedTests; }
        unsigned testFailedAsserts() const { return _testFailedAsserts; }
        unsigned suiteFailedTests() const { return _suiteFailedTests; }
        unsigned failedSuites() const { return _failedSuites; }
		
		unsigned unexpectedTests() const { return _unexpectedTests; }
        unsigned testUnexpectedAsserts() const { return _testUnexpectedAsserts; }
        unsigned suiteUnexpectedTests() const { return _suiteUnexpectedTests; }
        unsigned unexpectedSuites() const { return _unexpectedSuites; }
		
        void enterWorld( const WorldDescription &wd );
        void enterSuite( const SuiteDescription &sd );
        void enterTest( const TestDescription &td );
		
        void leaveTest( const TestDescription &td );
        void leaveSuite( const SuiteDescription &sd );
        void leaveWorld( const WorldDescription &wd );
		
        void tracedTest( const char *file, unsigned line, const char *expression );
        void warnedTest( const char *file, unsigned line, const char *expression );
        void failedTest( const char *file, unsigned line, const char *expression );
        void failedAssert( const char *file, unsigned line, const char *expression );
        void failedAssertEquals( const char *file, unsigned line,
                                 const char *xStr, const char *yStr,
                                 const char *x, const char *y );
        void failedAssertSameData( const char *file, unsigned line,
                                   const char *xStr, const char *yStr,
                                   const char *sizeStr, const void *x,
                                   const void *y, unsigned size );
        void failedAssertDelta( const char *file, unsigned line,
                                const char *xStr, const char *yStr, const char *dStr,
                                const char *x, const char *y, const char *d );
        void failedAssertDiffers( const char *file, unsigned line,
                                  const char *xStr, const char *yStr,
                                  const char *value );
        void failedAssertLessThan( const char *file, unsigned line,
                                   const char *xStr, const char *yStr,
                                   const char *x, const char *y );
        void failedAssertLessThanEquals( const char *file, unsigned line,
                                         const char *xStr, const char *yStr,
                                         const char *x, const char *y );
        void failedAssertPredicate( const char *file, unsigned line,
                                    const char *predicate, const char *xStr, const char *x );
        void failedAssertRelation( const char *file, unsigned line,
                                   const char *relation, const char *xStr, const char *yStr,
                                   const char *x, const char *y );
        void failedAssertThrows( const char *file, unsigned line,
                                 const char *expression, const char *type,
                                 bool otherThrown );
        void failedAssertThrowsNot( const char *file, unsigned line, const char *expression );
        
    private:
        TestTracker( const TestTracker & );
        TestTracker &operator=( const TestTracker & );

        static bool _created;
        TestListener _dummyListener;
        DummyWorldDescription _dummyWorld;
        unsigned _warnedTests, _testWarnedAsserts, _suiteWarnedTests, _warnedSuites;
		unsigned _failedTests, _testFailedAsserts, _suiteFailedTests, _failedSuites;
		unsigned _unexpectedTests, _testUnexpectedAsserts, _suiteUnexpectedTests, _unexpectedSuites;
        TestListener *_l;
        const WorldDescription *_world;
        const SuiteDescription *_suite;
        const TestDescription *_test;

        const TestDescription &dummyTest() const;
        const SuiteDescription &dummySuite() const;
        const WorldDescription &dummyWorld() const;
        
        void setWorld( const WorldDescription *w );
        void setSuite( const SuiteDescription *s );
        void setTest( const TestDescription *t );
        void countWarning();
        void countFailure();
		void countError();

        friend class TestRunner;
        
        TestTracker();
        void initialize();
        void setListener( TestListener *l );
    };

    inline TestTracker &tracker() { return TestTracker::tracker(); }
};

#endif // __cxxtest__TestTracker_h__
