#ifndef CCAPPLIB_CONDITION_VAR_H_
#define CCAPPLIB_CONDITION_VAR_H_

#include "ccapplib/env.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/thread/condition.h"

CCAPPLIB_BG

template<typename T>
class condition_var{
private:
	T _value;
	CCLIBC::mutex _mutex;
	CCLIBC::condition _cond;
	
public:
	condition_var FUNCTION_PARAMETER_LIST(const T &v):
		_value(v), _mutex(TRACE_PARAMS()), _cond(TRACE_PARAMS()){
	}
	
	//should only be invoked in the same thread of set value
	T value() const{
		return _value;
	}

	T state_value FUNCTION_PARAMETER_LIST(){
		GUARD_LOCK(_mutex);
		return _value;
	}

	void state_value FUNCTION_PARAMETER_LIST(const T &v){
		GUARD_LOCK(_mutex);
		_value = v;
		_cond.signal();
	}

	void wait_for FUNCTION_PARAMETER_LIST(const T &v){
		GUARD_LOCK(_mutex);
		while(_value != v){
			_cond.wait(_mutex);
		}
	}

	bool timed_wait_for FUNCTION_PARAMETER_LIST(const T &v, uint64_t ns){
		GUARD_LOCK(_mutex);
		while(_value != v && _cond.timed_wait(_mutex, ns));
		return _value == v;
	}

	bool is_value FUNCTION_PARAMETER_LIST(const T &v){
		GUARD_LOCK(_mutex);
		return v == _value;
	}
};

#ifdef STACK_TRACE_FLAG

#define state_value(...) state_value(INVOKE_TRACE, ##__VA_ARGS__)
#define wait_for(...) wait_for(INVOKE_TRACE, ##__VA_ARGS__)
#define timed_wait_for(...) timed_wait_for(INVOKE_TRACE, ##__VA_ARGS__)
#define is_value(...) is_value(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCAPPLIB_END

#endif //CCAPPLIB_CONDITION_VAR_H_
