#include <cstring>
#include "cclibc_test/exception_test.h"
#include "cclibc/trace.h"
#include "cclibc/exception.h"

using CCLIBC::exception;
using CCLIBC::stack;
using CCLIBC::stack_pool;
using CCLIBC::posix_error;
using CCLIBC::illegal_argument;

void exception_test_class::exception_properties() {
	exception e1; e1.trace("file", 1);
	exception e2; e2.trace("file2", 2, &e1);
	exception e3(""); e3.trace("file3", 3);
	exception e4(""); e4.trace("file4", 4, &e3);
	CPPUNIT_ASSERT(!strcmp(e1.file(), "file"));
	CPPUNIT_ASSERT(e1.line() == 1);
	CPPUNIT_ASSERT(!strcmp(e2.file(), "file2"));
	CPPUNIT_ASSERT(e2.line() == 2);
	CPPUNIT_ASSERT(e2.inner() == &e1);

	CPPUNIT_ASSERT(!strcmp(e3.file(), "file3"));
	CPPUNIT_ASSERT(e3.line() == 3);
	CPPUNIT_ASSERT(!strcmp(e4.file(), "file4"));
	CPPUNIT_ASSERT(e4.line() == 4);
	CPPUNIT_ASSERT(e4.inner() == &e3);
}

void exception_test_class::exception_message() {
	exception e("message");
	CPPUNIT_ASSERT(!strcmp(e.message(), "message"));

	exception e2("%d", 128);
	CPPUNIT_ASSERT(!strcmp(e2.message(), "128"));
}

void exception_test_class::exception_trace_1() {
#ifdef STACK_TRACE_FLAG
	const char *f1, *f2;
	int l1, l2;
	{
		f1 = __FILE__, l1 = __LINE__; STACK_TRACE();
		f2 = __FILE__, l2 = __LINE__; STACK_TRACE();
		exception e;
		e.trace(__FILE__, __LINE__);
		CPPUNIT_ASSERT(exception::last_stack().depth() == 2);
		CPPUNIT_ASSERT(!strcmp(exception::last_stack().recorder(1).file(), f1));
		CPPUNIT_ASSERT(exception::last_stack().recorder(1).line() == l1);

		CPPUNIT_ASSERT(!strcmp(exception::last_stack().recorder(0).file(), f2));
		CPPUNIT_ASSERT(exception::last_stack().recorder(0).line() == l2);
	}
#endif
}

void exception_test_class::exception_trace_2() {
#ifdef STACK_TRACE_FLAG
	const char *f1, *f2;
	int l1, l2;
	{
		exception e,e0,e1;
		f1 = __FILE__, l1 = __LINE__; STACK_TRACE();
		f2 = __FILE__, l2 = __LINE__; STACK_TRACE();
		e.trace(__FILE__, __LINE__);
		STACK_TRACE();
		e0.trace(__FILE__, __LINE__, &e1);
		CPPUNIT_ASSERT(exception::last_stack().depth() == 2);
		CPPUNIT_ASSERT(!strcmp(exception::last_stack().recorder(1).file(), f1));
		CPPUNIT_ASSERT(exception::last_stack().recorder(1).line() == l1);

		CPPUNIT_ASSERT(!strcmp(exception::last_stack().recorder(0).file(), f2));
		CPPUNIT_ASSERT(exception::last_stack().recorder(0).line() == l2);
	}
#endif
}

void exception_test_class::exception_setins(){
#ifdef STACK_TRACE_FLAG
	const stack *s = &exception::last_stack();
	stack ns;
	exception::set_last_stack(&ns);
	CPPUNIT_ASSERT(&ns == &exception::last_stack());
	exception::set_last_stack(const_cast<stack*>(s));
#endif
	
	const stack_pool *sp = &exception::get_exception_pool();
	stack_pool pl;
	exception::set_exception_pool(&pl);
	CPPUNIT_ASSERT(&pl == &exception::get_exception_pool());
	exception::set_exception_pool(const_cast<stack_pool*>(sp));
}

void exception_test_class::exception_throw_new_0(){
	const char *f;
	int l;
	try{
		f = __FILE__, l = __LINE__;	THROW_NEW_EXCEPTION(exception, "%d", 4096);
		CPPUNIT_ASSERT(!"Shoud not go to here.");
	}
	catch(const exception *e){
		CPPUNIT_ASSERT(!strcmp(e->message(), "4096"));
		CPPUNIT_ASSERT(!strcmp(e->file(), f));
		CPPUNIT_ASSERT(e->line() == l);
	}
	exception::clear_exception();
}

void exception_test_class::exception_throw_0(){
	const char *f;
	int l;
	exception te;
	try{
		f = __FILE__, l = __LINE__;	THROW_EXCEPTION(&te, exception, "%d", 4096);
		CPPUNIT_ASSERT(!"Shoud not go to here.");
	}
	catch(const exception *e){
		CPPUNIT_ASSERT(!strcmp(e->message(), "4096"));
		CPPUNIT_ASSERT(!strcmp(e->file(), f));
		CPPUNIT_ASSERT(e->line() == l);
		CPPUNIT_ASSERT(e->inner() == &te);
	}
	exception::clear_exception();
}

void exception_test_class::exception_throw_1() {
	const char *f;
	int l,l1;
#ifdef STACK_TRACE_FLAG
	size_t depth;
#endif
	try{
		f = __FILE__, l = __LINE__;	STACK_TRACE();
		try{
			THROW_NEW_EXCEPTION(exception);
		}catch(...){}
#ifdef STACK_TRACE_FLAG
		depth = exception::last_stack().depth();
#endif
		STACK_TRACE();
		exception te;
		l1 = __LINE__;	THROW_EXCEPTION(&te, exception, "%d", 4096);
		CPPUNIT_ASSERT(!"Shoud not go to here.");
	}
	catch(const exception *e){
#ifdef STACK_TRACE_FLAG
		CPPUNIT_ASSERT(exception::last_stack().depth() == depth);
		CPPUNIT_ASSERT(!strcmp(exception::last_stack().recorder(0).file(), f));
		CPPUNIT_ASSERT(exception::last_stack().recorder(0).line() == l);
#endif
		CPPUNIT_ASSERT(!strcmp(e->message(), "4096"));
		CPPUNIT_ASSERT(!strcmp(e->file(), f));
		CPPUNIT_ASSERT(e->line() == l1);
	}
	exception::clear_exception();
	l++;
}

void exception_test_class::exception_throw_new_1() {
	const char *f;
	int l,l1;
	try{
		STACK_TRACE();
		try{
			THROW_NEW_EXCEPTION(exception);
		}catch(...){}
		STACK_TRACE(); f = __FILE__, l = __LINE__;
		l1 = __LINE__; THROW_NEW_EXCEPTION(exception, "%d", 4096);
		CPPUNIT_ASSERT(!"Shoud not go to here.");
	}
	catch(const exception *e){
#ifdef STACK_TRACE_FLAG
		CPPUNIT_ASSERT(exception::last_stack().recorder(0).line() == l);
#endif
		CPPUNIT_ASSERT(!strcmp(e->message(), "4096"));
		CPPUNIT_ASSERT(!strcmp(e->file(), f));
		CPPUNIT_ASSERT(e->line() == l1);
	}
	exception::clear_exception();
	l++;
}


void exception_test_class::exception_throw_clear(){
	const void *p = NULL;
	exception te;
	try{
		THROW_EXCEPTION(&te, exception);
	}
	catch(const exception *e){
		p = e;
	}
	try{
		THROW_EXCEPTION(&te, exception);
	}
	catch(const exception *e){
		CPPUNIT_ASSERT(p != e);
	}
	exception::clear_exception();
}

void exception_test_class::exception_throw_new_clear(){
	const void *p = NULL;
	try{
		THROW_NEW_EXCEPTION(exception);
	}
	catch(const exception *e){
		p = e;
	}
	try{
		THROW_NEW_EXCEPTION(exception);
	}
	catch(const exception *e){
		CPPUNIT_ASSERT(p == e);
	}
	exception::clear_exception();
}

void exception_test_class::posix_error_properties() {
	posix_error e("xxx", 0);
	CPPUNIT_ASSERT(e.code() == 0);
	CPPUNIT_ASSERT(!strcmp(e.message(), "xxx: posix error (0): Success"));
	CPPUNIT_ASSERT(!strcmp(e.call(), "xxx"));
}

void exception_test_class::illegal_argument_properties(){
	illegal_argument e1("reason", "arg1", "function");
	CPPUNIT_ASSERT(!strcmp("function[arg1]: reason", e1.message()));
}

static bool exp1des = false;
static bool exp2des = false;

class Exp1: public exception{
public:
	~Exp1(){exp1des = true;}
};

class Exp2: public exception{
public:
	~Exp2(){exp2des = true;}
};

void exception_test_class::exception_destroy(){
	exp1des = false;
	exp2des = false;
	try{
		THROW_NEW_EXCEPTION(Exp1);
	}catch(const exception *e){
		try{
			THROW_EXCEPTION(e, Exp2);
		}
		catch(...){}
	}
	exception::clear_exception();
	CPPUNIT_ASSERT(exp1des);
	CPPUNIT_ASSERT(exp2des);
	exception::clear_exception();
}

void exception_test_class::exception_destroy1(){
	exp1des = false;
	exp2des = false;
	try{
		THROW_NEW_EXCEPTION(Exp1);
	}catch(const exception *e){
		try{
			THROW_EXCEPTION(e, Exp2);
		}
		catch(...){}
	}
	try{
		THROW_NEW_EXCEPTION(exception);
	}catch(...){}
	CPPUNIT_ASSERT(exp1des);
	CPPUNIT_ASSERT(exp2des);
	exception::clear_exception();
}

