#include <unistd.h>
#include <sys/time.h>
#include "cclibc_test/thread/mutex_test.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/thread/thread.h"

using CCLIBC::mutex;
using CCLIBC::thread;

class lock_test_obj{
public:
	int i;
	lock_test_obj(): m(TRACE_PARAMS()){}
	mutex m;
	void run(){
		GUARD_LOCK(m);
		if(i){
			i=1;
		}else{
			i=2;
		}
	}
};

void mutex_test_class::mutex_lock() {
	lock_test_obj o;
	o.i = 0;
	o.m.lock();
	thread t(TRACE_PARAMS(o,&lock_test_obj::run));
	usleep(10*1000);
	o.i = 10;
	o.m.unlock();
	t.join();
	CPPUNIT_ASSERT(1 == o.i);
}

static bool try_l;
void mutex_test_trylock_run(mutex &m){
	if((try_l = m.try_lock())){
		m.unlock();
	}
}

void mutex_test_class::mutex_trylock() {
	mutex TRACE_CONSTRUCT(m);
	{
		GUARD_LOCK(m);
		thread t(TRACE_PARAMS(mutex_test_trylock_run, m));
		t.join();
		CPPUNIT_ASSERT(!try_l);
	}
	thread t2(TRACE_PARAMS(mutex_test_trylock_run, m));
	t2.join();
	CPPUNIT_ASSERT(try_l);
}

static int test_i = 0;

void mutex_guardtrylock_run(mutex &m){
	if(GUARD_TRYLOCK(m)){
		test_i = 1;
	}else{
		test_i = 2;
	}
}

void mutex_test_class::mutex_guardtrylock() {
	mutex TRACE_CONSTRUCT(m);
	{
		GUARD_LOCK(m);
		thread t(TRACE_PARAMS(mutex_guardtrylock_run, m));
		usleep(10*1000);
		t.join();
		CPPUNIT_ASSERT(test_i == 2);
	}
	thread t2(TRACE_PARAMS(mutex_guardtrylock_run, m));
	usleep(10*1000);
	t2.join();
	CPPUNIT_ASSERT(test_i == 1);
}

static uint64_t s;
static bool time_l;
void mutex_timedlock_block_run(mutex &m){
	timeval t1, t2;
	gettimeofday(&t1, NULL);
	if(GUARD_TIMEDLOCK(m, 100000000)){
		time_l = true;
	}else{
		time_l = false;
	}
	gettimeofday(&t2, NULL);
	s = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
}

void mutex_test_class::mutex_timedlock_block() {
	mutex TRACE_CONSTRUCT(m);
	GUARD_LOCK(m);
	thread t(TRACE_PARAMS(mutex_timedlock_block_run, m));
	t.join();
	CPPUNIT_ASSERT_EXT( s <120 && s > 80 , "%d", (int)s);
	CPPUNIT_ASSERT(!time_l);
}

void mutex_timedlock_lock_run(mutex &m){
	timeval t1, t2;
	gettimeofday(&t1, NULL);
	if(GUARD_TIMEDLOCK(m, 100000000)){
		time_l = true;
	}else{
		time_l = false;
	}
	gettimeofday(&t2, NULL);
	s = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
}

void mutex_test_class::mutex_timedlock_lock() {
	mutex TRACE_CONSTRUCT(m);
	m.lock();
	thread t(TRACE_PARAMS(mutex_timedlock_block_run, m));
	usleep(50*1000);
	m.unlock();
	t.join();
	CPPUNIT_ASSERT_EXT( s <60 && s > 40 , "%d", (int)s);
	CPPUNIT_ASSERT(time_l);
}
