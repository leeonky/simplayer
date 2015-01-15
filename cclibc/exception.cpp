#include "cclibc/exception.h"
CCLIBC_BG
__thread exception* exception::_last_exception = NULL;
__thread stack* exception::_last_stack = NULL;
__thread stack_pool* exception::_pool = NULL;

exception::exception(const char *fmt, ...):_file(""), _line(0), _inner(NULL), _message("") {
	if(fmt && strcmp(fmt,"")){
		va_list list;
		va_start(list, fmt);
		int len = vsnprintf(NULL, 0, fmt, list);
		va_end(list);
		debug_assert(len >= 0);
		char *msg_buf = static_cast<char*>(get_exception_pool().allocate_nothrow(++len));
		if(msg_buf){
			va_start(list, fmt);
			vsprintf(msg_buf, fmt, list);
			va_end(list);
			_message = msg_buf;
		}else{
			_message = "Message is too long.";
		}
	}
}

void exception::trace(const char *f, int l, const exception *i){
	_file = f;
	_line = l;
	_inner = i;
	_last_exception = this;
#ifdef STACK_TRACE_FLAG
	if(!i){
		exception::get_last_stack() = stack::running_stack();
	}
#endif
}

CCLIBC_END

