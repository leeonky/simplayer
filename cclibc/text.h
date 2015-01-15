#ifndef CCLIBC_TEXT_H_
#define CCLIBC_TEXT_H_

#include "cclibc/env.h"
#include "cclibc/trace.h"

CCLIBC_BG

class encoding{
public:
	static size_t transform_format FUNCTION_PARAMETER_LIST(const char*, const char*, const void*, size_t, void*, size_t);
	static const char* detect_encoding FUNCTION_PARAMETER_LIST(const char*);
	static const char* detect_encoding FUNCTION_PARAMETER_LIST(const char*, size_t, const char**);
};

#ifdef STACK_TRACE_FLAG

#define transform_format(...) transform_format(INVOKE_TRACE,## __VA_ARGS__)
#define detect_encoding(...) detect_encoding(INVOKE_TRACE,## __VA_ARGS__)

#endif

CCLIBC_END

#endif //CCLIBC_TEXT_H_
