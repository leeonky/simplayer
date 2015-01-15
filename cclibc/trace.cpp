#include "cclibc/trace.h"

CCLIBC_BG

__thread stack* stack::_running_stack = NULL;

void stack::print_stack(FILE *fp) const{
	if(!fp){
		fp = stderr;
	}
	stack s = *this;
	for(size_t i = 0; i<s.depth(); i++){
		const stack_recorder &r = s.recorder(i);
		fprintf(fp, "\tat %s:%d\n", r.file(), r.line());
	}
}

CCLIBC_END
