#include "cclibc/assert.h"
#include "cclibc/trace.h"

CCLIBC_BG

int assert_util::assert_print(const char *exp, const char *fun, const char *f, int l){
	FILE *fp = stderr;
	fprintf(fp, "\nAssertion '%s' failed at %s(%s:%d)\n", exp, fun, f, l);
#ifdef STACK_TRACE_FLAG
	stack::running_stack().print_stack(fp);
#endif	
	abort();
	return -1;
}

CCLIBC_END
