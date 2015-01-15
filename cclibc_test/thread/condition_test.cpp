#include <unistd.h>
#include <sys/time.h>
#include "cclibc_test/thread/condition_test.h"
#include "cclibc/thread/condition.h"
#include "cclibc/thread/thread.h"

using CCLIBC::condition;
using CCLIBC::mutex;
using CCLIBC::thread;

class test_obj{
public:
	condition cond;
	mutex m;
	uint64_t s;
	bool timeout;
	int value;
	test_obj(): cond(TRACE_PARAMS()), m(TRACE_PARAMS()){}

	void wait_run(){
		GUARD_LOCK(m);
		timeval t1, t2;
		gettimeofday(&t1, NULL);
		cond.wait(m);
		gettimeofday(&t2, NULL);
		s = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
	}

	void timedwait_run(){
		GUARD_LOCK(m);
		timeval t1, t2;
		gettimeofday(&t1, NULL);
		timeout = !cond.timed_wait(m, 1000*1000*2000);
		gettimeofday(&t2, NULL);
		s = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
	}
};

void condition_test_class::condition_wait(){
	test_obj o;
	thread t(TRACE_PARAMS(o,&test_obj::wait_run));
	usleep(10*1000);
	{
		GUARD_LOCK(o.m);
		usleep(100*1000);
	}
	o.cond.signal();
	t.join();
	CPPUNIT_ASSERT_EXT( o.s <120 && o.s > 80 , "%d", (int)o.s);
}

void condition_test_class::condition_timedwait() {
	test_obj o;
	thread t(TRACE_PARAMS(o, &test_obj::timedwait_run));
	usleep(10*1000);
	{
		GUARD_LOCK(o.m);
		usleep(100*1000);
	}
	o.cond.signal();
	t.join();
	CPPUNIT_ASSERT_EXT( o.s <120 && o.s > 80 , "%d", (int)o.s);
	CPPUNIT_ASSERT(!o.timeout);
	{
		GUARD_LOCK(o.m);
		timeval t1, t2;
		gettimeofday(&t1, NULL);
		CPPUNIT_ASSERT(!o.cond.timed_wait(o.m, 1000*1000*100));
		gettimeofday(&t2, NULL);
		uint64_t s = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
		CPPUNIT_ASSERT_EXT(s <120 && s > 80 , "%d", (int)s);
	}
}

void condition_test_class::condition_broadcast() {

}
