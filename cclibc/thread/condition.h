#ifndef CCLIBC_THREAD_CONDITION_H_
#define CCLIBC_THREAD_CONDITION_H_

#include <pthread.h>
#include "cclibc/env.h"
#include "cclibc/assert.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/trace.h"
#include "cclibc/exception.h"
#include "cclibc/callc.h"

CCLIBC_BG

#ifdef STACK_TRACE_FLAG

#ifndef wait
#define wait(...) wait(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#ifndef timed_wait
#define timed_wait(...) timed_wait(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#define signal(...) signal(INVOKE_TRACE, ##__VA_ARGS__)
#define timed_wait(...) timed_wait(INVOKE_TRACE, ##__VA_ARGS__)

#endif

class condition{
private:
	pthread_cond_t _cond;
	
public:
	condition FUNCTION_PARAMETER_LIST() {
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_cond_init, &_cond, NULL);
	}

	~condition() {
		debug_assert(!pthread_cond_destroy(&_cond));
	}

	void wait FUNCTION_PARAMETER_LIST(mutex &m) {
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_cond_wait, &_cond, &m._mutex);
	}

	void signal FUNCTION_PARAMETER_LIST() {
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_cond_signal, &_cond);
	}

	bool timed_wait FUNCTION_PARAMETER_LIST(mutex &m, uint64_t ns) {
		const long nanos_per_s = 1000000000;
		timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
		t.tv_sec += ns / nanos_per_s;
		t.tv_nsec += (ns % nanos_per_s);
		if (t.tv_nsec >= nanos_per_s) {
			t.tv_sec++;
			t.tv_nsec -= nanos_per_s;
		}
		switch(int c = pthread_cond_timedwait(&_cond, &m._mutex, &t)){
			case ETIMEDOUT:
			case 0:
				return !c;
			default:
				THROW_NEW_EXCEPTION(posix_error, "pthread_cond_timedwait", c);
		} 
	}
};

CCLIBC_END

#endif //CCLIBC_THREAD_CONDITION_H_
