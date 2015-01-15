#include <sys/time.h>
#include <cppunit/Protector.h>
#include <cstdio>
#include <stdint.h>
#include <cstdarg>
#include "cctest/util.h"
#include "cclibc/exception.h"

using CCLIBC::exception;
using CCLIBC::allocator;

std::vector<method_invoke> interface_testor::_methods;
bool interface_testor::record_stack = true;

TestTimer::TestTimer(const char *f, int l, const char *n): name(n){
	gettimeofday(&tst, NULL);
	printf("%s start at %s:%d\n", name, f, l);
}
TestTimer::~TestTimer(){
	timeval end;
	gettimeofday(&end, NULL);
	uint64_t s1 = end.tv_sec;
	s1 = s1 * 1000000 + end.tv_usec;
	uint64_t s2 = tst.tv_sec;
	s2 = s2 * 1000000 + tst.tv_usec;
	printf("%s done after %d ms.\n", name, (int)((s1-s2)/1000));
}

CPPUNIT_NS_BEGIN

class CCLIBCTestProtector: public Protector{
public:
	bool protect( const Functor &functor,
                const ProtectorContext &context ){
  try
  {
    return functor();
  }
  catch ( Exception &failure )
  {
    reportFailure( context, failure );
  }
  catch ( std::exception &e )
  {
    std::string shortDescription( "uncaught exception of type " );
#if CPPUNIT_USE_TYPEINFO_NAME
    shortDescription += TypeInfoHelper::getClassName( typeid(e) );
#else
    shortDescription += "std::exception (or derived).";
#endif
    Message message( shortDescription, e.what() );
    reportError( context, message );
  }
  catch (const exception *e){
	printf("Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
#ifdef STACK_TRACE_FLAG
	exception::last_stack().print_stack(stdout);
#endif
	reportError( context, Message("Uncaught exception.") );
  }
  catch ( ... )
  {
    reportError( context,
                 Message( "uncaught exception of unknown type") );
  }
  return false;
	}
};

CPPUNIT_NS_END

int do_all_test(const char *name){
	TIMER_ON(name);
	CPPUNIT_NS::TestResult controller;
	controller.pushProtector(new CPPUNIT_NS::CCLIBCTestProtector());
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener(&result);
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener(&progress);
	CPPUNIT_NS::TestRunner runner;
	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run(controller);
	CPPUNIT_NS::CompilerOutputter outputter(&result, CPPUNIT_NS::stdCOut());
	outputter.write();
	return result.wasSuccessful() ? 0 : 1;
}

void* test_util_allocator::allocate FUNCTION_PARAMETER_LIST(size_t s){
	_allocated+=s;
	size_t *p = static_cast<size_t*>(allocator::instance().allocate(s+sizeof(size_t)));
	*p = s;
	return p+1;
}
char* test_util_allocator::allocate FUNCTION_PARAMETER_LIST(const char *fmt, ...){
	va_list list;
	va_start(list, fmt);
	int len = vsnprintf(NULL, 0, fmt, list);
	va_end(list);
	debug_assert(len >= 0);
	char *buf = static_cast<char*>(allocate(static_cast<size_t>(++len)));
	va_start(list, fmt);
	debug_assert(vsnprintf(buf, len, fmt, list));
	va_end(list);
	return buf;
}
void test_util_allocator::deallocate FUNCTION_PARAMETER_LIST(void *p){
	if(p){
		size_t *rp = static_cast<size_t*>(p) - 1;
		_allocated-=*rp;
		allocator::instance().deallocate(rp);
	}
}

