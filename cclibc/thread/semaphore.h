#ifndef CCLIBC_THREAD_SEMAPHORE_H_
#define CCLIBC_THREAD_SEMAPHORE_H_

#include <semaphore.h>
#include "cclibc/env.h"
#include "cclibc/assert.h"
#include "cclibc/exception.h"
#include "cclibc/callc.h"
#include "cclibc/trace.h"

CCLIBC_BG

#ifdef STACK_TRACE_FLAG

#ifndef wait
#define wait(...) wait(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#define post(...) post(INVOKE_TRACE, ##__VA_ARGS__)

#define try_wait(...) try_wait(INVOKE_TRACE, ##__VA_ARGS__)

#ifndef timed_wait
#define timed_wait(...) timed_wait(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#endif

class semaphore{
private:
	sem_t _sem;
public:
	semaphore FUNCTION_PARAMETER_LIST(unsigned value = 0, bool shared = false) {
		memset(&_sem, 0, sizeof(sem_t ));
		VOID_INVOKE_THROW(!=0, posix_error, sem_init, &_sem, shared, value);
	}

	~semaphore() {
		debug_assert(!sem_destroy(&_sem));
	}

	void wait FUNCTION_PARAMETER_LIST(){
		VOID_INVOKE_THROW(!=0, posix_error, sem_wait, &_sem);
	}

	void post FUNCTION_PARAMETER_LIST(){
		VOID_INVOKE_THROW(!=0, posix_error, sem_post, &_sem);
	}

	bool try_wait FUNCTION_PARAMETER_LIST() {
		if(sem_trywait(&_sem)){
			switch(int c = errno){
				case EAGAIN:
					return false;
				default:
					THROW_NEW_EXCEPTION(posix_error, "sem_trywait", c);
			}
		}
		return true;
	}

	bool timed_wait FUNCTION_PARAMETER_LIST(uint64_t ns) {
		const long nanos_per_s = 1000000000;
		timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
		t.tv_sec += ns / nanos_per_s;
		t.tv_nsec += (ns % nanos_per_s);
		if (t.tv_nsec >= nanos_per_s) {
			t.tv_sec++;
			t.tv_nsec -= nanos_per_s;
		}
		if(sem_timedwait(&_sem, &t)){
			switch(int c = errno){
				case ETIMEDOUT:
					return false;
				default:
					THROW_NEW_EXCEPTION(posix_error, "sem_timedwait", c);
			}
		}
		return true;
	}
};

CCLIBC_END

#endif //CCLIBC_THREAD_SEMAPHORE_H_
