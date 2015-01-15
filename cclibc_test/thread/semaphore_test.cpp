#include <unistd.h>
#include <sys/time.h>
#include "cclibc_test/thread/semaphore_test.h"
#include "cclibc/thread/semaphore.h"
#include "cclibc/thread/thread.h"

using CCLIBC::semaphore;
using CCLIBC::thread;

class sem_run_obj{
public:
	sem_run_obj(): s(TRACE_PARAMS()){}
	semaphore s;
	int i,j;
	void run_wait(){
		i = 1;
		s.wait();
		if(i){
			i = 2;
		}else{
			i = 3;
		}
	}
};

void semaphore_test_class::semaphore_wait() {
	sem_run_obj o;
	thread t(TRACE_PARAMS(o, &sem_run_obj::run_wait));
	usleep(10*1000);
	o.i = 0;
	o.s.post();
	t.join();
	CPPUNIT_ASSERT(3 == o.i);
}

void semaphore_test_class::semaphore_trywait() {
	semaphore TRACE_CONSTRUCT(s);
	CPPUNIT_ASSERT(!s.try_wait());
	s.post();
	CPPUNIT_ASSERT(s.try_wait());
}

void semaphore_test_class::semaphore_timedwait() {
	semaphore TRACE_CONSTRUCT(s);
	timeval t1, t2;
	gettimeofday(&t1, NULL);
	CPPUNIT_ASSERT(!s.timed_wait(100000000));
	gettimeofday(&t2, NULL);
	uint64_t ss = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
	CPPUNIT_ASSERT_EXT( ss<120 && ss>80, "%d", (int)ss);
	s.post();
	CPPUNIT_ASSERT(s.timed_wait(100000000));
}

