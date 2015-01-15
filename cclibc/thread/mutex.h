#ifndef CCLIBC_THREAD_MUTEX_H_
#define CCLIBC_THREAD_MUTEX_H_

#include <ctime>
#include <pthread.h>
#include "cclibc/env.h"
#include "cclibc/assert.h"
#include "cclibc/exception.h"
#include "cclibc/trace.h"
#include "cclibc/callc.h"

#ifdef STACK_TRACE_FLAG
#ifndef lock
#define lock(...) lock(INVOKE_TRACE, ##__VA_ARGS__)
#endif
#ifndef unlock
#define unlock(...) unlock(INVOKE_TRACE, ##__VA_ARGS__)
#endif
#define try_lock(...) try_lock(INVOKE_TRACE, ##__VA_ARGS__)
#define timed_lock(...) timed_lock(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCLIBC_BG

class condition;
class mutex{
friend class condition;
private:
	pthread_mutex_t _mutex;
public:
	mutex FUNCTION_PARAMETER_LIST(){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_mutex_init, &_mutex, NULL);
	}

	~mutex(){
		debug_assert(!pthread_mutex_destroy(&_mutex));
	}

	void lock FUNCTION_PARAMETER_LIST(){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_mutex_lock, &_mutex);
	}

	void unlock FUNCTION_PARAMETER_LIST(){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_mutex_unlock, &_mutex);
	}

	bool try_lock FUNCTION_PARAMETER_LIST(){
		bool r = true;
		switch(int c = pthread_mutex_trylock(&_mutex)){
			case EBUSY:
				r = false;
			case 0:
				return r;
			default:
				THROW_NEW_EXCEPTION(posix_error, "pthread_mutex_trylock", c);
		}
	}

	bool timed_lock FUNCTION_PARAMETER_LIST(uint64_t ns){
		const long nanos_per_s = 1000000000;
		timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
		t.tv_sec += ns / nanos_per_s;
		t.tv_nsec += (ns % nanos_per_s);
		if (t.tv_nsec >= nanos_per_s) {
			t.tv_sec++;
			t.tv_nsec -= nanos_per_s;
		}
		switch(int c = pthread_mutex_timedlock(&_mutex, &t)){
			case ETIMEDOUT:
			case 0:
				return !c;
			default:
				THROW_NEW_EXCEPTION(posix_error, "pthread_mutex_timedlock", c);
		}
	}
};

class lock_guard{
private:
	mutex &_m;
public:
	lock_guard FUNCTION_PARAMETER_LIST(mutex &m):_m(m){
		_m.lock();
	}

	~lock_guard() {
		_m.unlock();
	}
};

class trylock_guard{
private:
	mutex &_m;
	mutable bool _locked;
public:
	trylock_guard FUNCTION_PARAMETER_LIST(mutex &m): _m(m), _locked(m.try_lock()){}

	~trylock_guard(){
		if(_locked){
			_m.unlock();
		}
	}
	
	operator bool() {
		return _locked;
	}

	trylock_guard(const trylock_guard &other) : _m(other._m), _locked(other._locked) {
		if(other._locked) {
			other._locked = false;
		}
	}
};

class timedlock_guard{
private:
	mutex &_m;
	mutable bool _locked;
public:
	timedlock_guard FUNCTION_PARAMETER_LIST(mutex &m, uint64_t ns): _m(m), _locked(m.timed_lock(ns)){}
	~timedlock_guard(){
		if(_locked){
			_m.unlock();
		}
	}
	
	operator bool() {
		return _locked;
	}

	timedlock_guard(const timedlock_guard &other) : _m(other._m), _locked(other._locked) {
		if(other._locked) {
			other._locked = false;
		}
	}
};

CCLIBC_END

#define GUARD_LOCK(m) CCLIBC::lock_guard UNIQUE_INDENTIFIER(__lock_guard, __COUNTER__) (TRACE_PARAMS(m))

#define GUARD_TRYLOCK(m) CCLIBC::trylock_guard UNIQUE_INDENTIFIER(__trylock_guard, __COUNTER__) = CCLIBC::trylock_guard(TRACE_PARAMS(m))

#define GUARD_TIMEDLOCK(m,ns) CCLIBC::timedlock_guard UNIQUE_INDENTIFIER(__timedlock_guard, __COUNTER__) = CCLIBC::timedlock_guard(TRACE_PARAMS(m, ns))

#endif //CCLIBC_THREAD_MUTEX_H_
