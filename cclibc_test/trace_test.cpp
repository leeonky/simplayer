#include <cstring>
#include "cclibc_test/trace_test.h"
#include "cclibc/trace.h"

using CCLIBC::stack_recorder;
using CCLIBC::stack;
using CCLIBC::trace_guard;

void trace_test_class::stack_recorder_properties() {
	const char *file = __FILE__;
	int line = __LINE__;
	stack_recorder s;
	s.record(file, line);
	CPPUNIT_ASSERT(s.file() == file);
	CPPUNIT_ASSERT(s.line() == line);
}

void trace_test_class::stack_properties() {
	stack s;
	CPPUNIT_ASSERT(!s.depth());
}

static stack st_trace;

void trace_test_class::stack_trace() {
	st_trace.enter("file1", 1);
	CPPUNIT_ASSERT(st_trace.depth() == 1);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(0).file(), "file1"));
	CPPUNIT_ASSERT(st_trace.recorder(0).line() == 1);

	st_trace.enter("file2", 2);
	CPPUNIT_ASSERT(st_trace.depth() == 2);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(0).file(), "file2"));
	CPPUNIT_ASSERT(st_trace.recorder(0).line() == 2);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(1).file(), "file1"));
	CPPUNIT_ASSERT(st_trace.recorder(1).line() == 1);

	st_trace.enter("file3", 3);
	CPPUNIT_ASSERT(st_trace.depth() == 3);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(0).file(), "file3"));
	CPPUNIT_ASSERT(st_trace.recorder(0).line() == 3);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(1).file(), "file2"));
	CPPUNIT_ASSERT(st_trace.recorder(1).line() == 2);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(2).file(), "file1"));
	CPPUNIT_ASSERT(st_trace.recorder(2).line() == 1);

	st_trace.leave();
	CPPUNIT_ASSERT(st_trace.depth() == 2);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(0).file(), "file2"));
	CPPUNIT_ASSERT(st_trace.recorder(0).line() == 2);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(1).file(), "file1"));
	CPPUNIT_ASSERT(st_trace.recorder(1).line() == 1);

	st_trace.leave();
	CPPUNIT_ASSERT(st_trace.depth() == 1);
	CPPUNIT_ASSERT(!strcmp(st_trace.recorder(0).file(), "file1"));
	CPPUNIT_ASSERT(st_trace.recorder(0).line() == 1);

	st_trace.leave();
	CPPUNIT_ASSERT(!st_trace.depth());
}

static stack st_copy;
void trace_test_class::stack_copy() {
	st_copy.enter("file1", 1);
	st_copy.enter("file2", 2);
	stack st = st_copy;

	CPPUNIT_ASSERT(st.depth() == 2);
	CPPUNIT_ASSERT(!strcmp(st.recorder(0).file(), "file2"));
	CPPUNIT_ASSERT(st.recorder(0).line() == 2);
	CPPUNIT_ASSERT(!strcmp(st.recorder(1).file(), "file1"));
	CPPUNIT_ASSERT(st.recorder(1).line() == 1);
}

void trace_test_class::stack_static() {
#ifdef STACK_TRACE_FLAG
	const stack *p = &stack::running_stack();
	stack s;
	stack::set_running_stack(&s);
	CPPUNIT_ASSERT(&s == &stack::running_stack());
	stack::set_running_stack(const_cast<stack*>(p));
#endif
}

void trace_test_class::stack_trace_guard() {
#ifdef STACK_TRACE_FLAG
	{
		trace_guard g("file", 1);
		stack s = stack::running_stack();
		CPPUNIT_ASSERT(s.depth() == 1);
		CPPUNIT_ASSERT(!strcmp(s.recorder(0).file(), "file"));
		CPPUNIT_ASSERT(s.recorder(0).line() == 1);
	}
	CPPUNIT_ASSERT(!stack::running_stack().depth());
#endif
}

void trace_test_class::stack_trace_trace_macro() {
#ifdef STACK_TRACE_FLAG
	const char *f;
	int l;
	{
		f = __FILE__, l = __LINE__; STACK_TRACE();
		stack s = stack::running_stack();
		CPPUNIT_ASSERT(s.depth() == 1);
		CPPUNIT_ASSERT(s.recorder(0).file() == f);
		CPPUNIT_ASSERT(s.recorder(0).line() == l);
	}
	CPPUNIT_ASSERT(!stack::running_stack().depth());
#endif
}

