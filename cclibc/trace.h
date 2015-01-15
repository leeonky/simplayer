#ifndef CCLIBC_TRACE_H_
#define CCLIBC_TRACE_H_


#include <cstring>
#include <cstdio>
#include "cclibc/env.h"
#include "cclibc/assert.h"

#ifdef STACK_TRACE_FLAG

#define STACK_TRACE() CCLIBC::trace_guard UNIQUE_INDENTIFIER(__trace_guard_, __COUNTER__) (__FILE__ ,__LINE__)
#define INVOKE_TRACE CCLIBC::trace_guard(__FILE__ ,__LINE__)

#define FUNCTION_PARAMETER_LIST(...) (const CCLIBC::trace_guard&, ## __VA_ARGS__)
#define TRACE_CONSTRUCT(var)	var(INVOKE_TRACE)
#define TRACE_PARAMS(...) INVOKE_TRACE, ## __VA_ARGS__

#else

#define STACK_TRACE()
#define INVOKE_TRACE

#define FUNCTION_PARAMETER_LIST(...) (__VA_ARGS__)
#define TRACE_CONSTRUCT(var) var
#define TRACE_PARAMS(...) __VA_ARGS__

#endif

CCLIBC_BG

class stack_recorder {
private:
	const char *_file;
	int _line;

public:
	void record(const char *f, int l) {
		_file = f;
		_line = l;
	}
	
	const char* file() const{
		return _file;
	}

	int line() const {
		return _line;
	}
};

#ifndef MAX_STACK_DEPTH
#define MAX_STACK_DEPTH 512
#endif

class trace_guard;
class stack{
friend class trace_guard;
private:
	size_t _depth;
	stack_recorder _recorders[MAX_STACK_DEPTH];
	static __thread stack *_running_stack;
	static stack& get_running_stack() {
		if(!_running_stack){
			static stack s;
			_running_stack = &s;
		}
		return *_running_stack;
	}

public:
	stack(): _depth(0) { }

	stack(const stack &other): _depth(other._depth) {
		memcpy(_recorders, other._recorders, sizeof(stack_recorder) * _depth);
	}

	void enter(const char *f, int l){
		release_assert(_depth < MAX_STACK_DEPTH);
		_recorders[_depth++].record(f, l);
	}

	void leave() {
		--_depth;
	}

	size_t depth() const {
		return _depth;
	}
	
	const stack_recorder& recorder FUNCTION_PARAMETER_LIST(size_t d) const {
		size_t p = depth() - d - 1;
		debug_assert(p < MAX_STACK_DEPTH);
		return _recorders[p];
	}

	void print_stack(FILE *fp = stderr) const;

#ifdef STACK_TRACE_FLAG
	static void set_running_stack(stack *s) {
		_running_stack = s;
	}
	
	static const stack& running_stack() {
		return get_running_stack();
	}
#endif
};

#ifdef STACK_TRACE_FLAG

class trace_guard{
private:
	stack &_stack;
public:
	trace_guard(const char *f, int l) : _stack(stack::get_running_stack()) {
		_stack.enter(f, l);
	}
	~trace_guard() {
		_stack.leave();
	}
};

#define recorder(...) recorder(INVOKE_TRACE, ## __VA_ARGS__)

#endif


CCLIBC_END

#endif //CCLIBC_TRACE_H_
