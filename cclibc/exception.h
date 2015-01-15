#ifndef CCLIBC_EXCEPTION_H_
#define CCLIBC_EXCEPTION_H_

#include <new>
#include <cerrno>
#include <cstring>
#include <cstdarg>
#include "cclibc/env.h"
#include "cclibc/trace.h"
#include "cclibc/memory.h"
#include "cclibc/assert.h"

CCLIBC_BG

class exception{
private:
	const char *_file;
	int _line;
	const exception *_inner;
	const char *_message;

public:
	exception(const char *fmt = NULL, ...);

	virtual ~exception() {
		if(_inner){
			_inner->~exception();
		}
	}
	const char* file() const {
		return _file;
	}

	int line() const {
		return _line;
	}

	const exception* inner() const {
		return _inner;
	}

	const char* message() const {
		return _message;
	}

	void trace(const char*, int, const exception *i = NULL);
	
private:
	static __thread exception *_last_exception;
	static __thread stack *_last_stack;
	static stack& get_last_stack() {
		if(!_last_stack){
			static stack s;
			_last_stack = &s;
		}
		return *_last_stack;
	}
	static __thread stack_pool *_pool;

public:
#ifdef STACK_TRACE_FLAG
	static void set_last_stack(stack *s) {
		_last_stack = s;
	}
	
	static const stack& last_stack() {
		return get_last_stack();
	}
#endif
	static stack_pool& get_exception_pool(){
		if(!_pool){
			static stack_pool s;
			_pool = &s;
		}
		return *_pool;
	}
	static void set_exception_pool(stack_pool *s){
		_pool = s;
	}
	
	static const exception* last_exception() {
		return _last_exception;
	}

	static void clear_exception() {
		if(_last_exception){
			_last_exception->~exception();
			_last_exception = NULL;
		}
		get_exception_pool().reset();
	}
};

class stack_overflow: public exception{
public:
	stack_overflow():exception("Stack trace overflow"){}
};

class buffer_overflow: public exception{
public:
	buffer_overflow():exception("Buffer overflow"){}
};

class posix_error: public exception{
private:
	const char *_call;
	int _code;
public:
	posix_error(const char *call): exception("%s: posix error (%d): %s", call, errno, strerror(errno)),_call(call), _code(errno){}
	posix_error(const char *call, int c): exception("%s: posix error (%d): %s", call, c, strerror(c)), _call(call), _code(c){}
	int code() const {
		return _code;
	}

	const char* call() {
		return _call;
	}

	static int error() {
		return errno;
	}
};

class illegal_argument: public exception{
public:
	illegal_argument(const char *reason, const char *arg, const char *function):
		exception("%s[%s]: %s", function, arg, reason){}
};

#define THROW_ILLEGAL_ARGUMENT(msg, var_name) THROW_NEW_EXCEPTION(CCLIBC::illegal_argument, msg, var_name, __PRETTY_FUNCTION__)

#define ILLEGAL_ARGUMENT_CHECK(exp, var) do{\
	if(!(exp)){\
		THROW_ILLEGAL_ARGUMENT(#exp, #var);\
	}\
	}while(0)

class allocate_error: public exception{
};

#define THROW_EXCEPTION(inner, exp_type, ...) do{\
	const void *_p = inner;\
	if(!_p){\
		CCLIBC::exception::clear_exception();\
	}\
	exp_type *_exception_p = STACK_NEW_NOTHROW(CCLIBC::exception::get_exception_pool(), exp_type, __VA_ARGS__); \
	release_assert(_exception_p);\
	(new(_exception_p)exp_type(__VA_ARGS__))->trace(__FILE__, __LINE__, inner);\
	throw const_cast<const exp_type*>(_exception_p);\
} while(0)

#define THROW_NEW_EXCEPTION(exp_type, ...) THROW_EXCEPTION(NULL, exp_type, ## __VA_ARGS__);

CCLIBC_END

#endif //CCLIBC_EXCEPTION_H_
