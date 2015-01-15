#include <sys/time.h>
#include <unistd.h>
#include "cclibc_test/thread/thread_test.h"
#include "cclibc/thread/thread.h"

using CCLIBC::thread_attribute;
using CCLIBC::thread;
using CCLIBC::stack;
using CCLIBC::exception;
using CCLIBC::posix_error;

void thread_test_class::thread_attribute_set_stack_size() {
	thread_attribute TRACE_CONSTRUCT(attr);
	size_t s = attr.stack_size();
	attr.stack_size(s + 1024*1024);
	CPPUNIT_ASSERT(attr.stack_size() == s + 1024*1024);
}

void thread_test_class::thread_attribute_detach() {
	thread_attribute TRACE_CONSTRUCT(attr);
	bool f = attr.detached();
	attr.detached(!f);
	CPPUNIT_ASSERT(attr.detached() == !f);
}

namespace {
	class destructor{
	public:
		int &_i;
		destructor(int &i):_i(i){}
		~destructor(){_i = 0; }

		static void run_cancel(int &i) {
			destructor d(i);
			for(;;){
				usleep(10000*1000);
			}
		}

		static void run_exit(int &i) {
			destructor d(i);
			for(;;){
				thread::exit();
			}
		}
	};

	static void thread_run_sleep() {
		usleep(1000*100);
	}
}

void thread_test_class::thread_interrupt() {
	int i=100;
	thread t(TRACE_PARAMS(destructor::run_cancel, i));
	t.cancel();
	t.join();
	CPPUNIT_ASSERT(i == 0);
}

void thread_test_class::thread_exit() {
	int i=200;
	thread t(TRACE_PARAMS(destructor::run_exit, i));
	t.join();
	CPPUNIT_ASSERT(i == 0);
}

void thread_test_class::thread_sleep() {
	timeval t1, t2;
	gettimeofday(&t1, NULL);
	uint64_t ns = 100000000;
	thread::sleep_nano(ns);
	gettimeofday(&t2, NULL);
	int64_t s = (t2.tv_sec * 1000 + t2.tv_usec/1000 - (t1.tv_sec * 1000 + t1.tv_usec/1000));
	CPPUNIT_ASSERT_EXT( s <120 && s > 80 , "%d", (int)s);

}

#ifdef STACK_TRACE_FLAG
static const stack *s1, *es1;
#endif
static const exception *p1;

static void thread_thread_object_run(){
#ifdef STACK_TRACE_FLAG
	s1 = &stack::running_stack();
#endif
	try{
		exception::clear_exception();
		THROW_NEW_EXCEPTION(exception);
	}catch(const exception *e){
		p1 = e;
	}
#ifdef STACK_TRACE_FLAG
	es1 = &exception::last_stack();
#endif
}

void thread_test_class::thread_thread_object(){
#ifdef STACK_TRACE_FLAG
	const stack &s = stack::running_stack();
#endif
	const exception *p = NULL;
	try{
		exception::clear_exception();
		THROW_NEW_EXCEPTION(exception);
	}catch(const exception *e){
		p = e;
	}
#ifdef STACK_TRACE_FLAG
	const stack &es = exception::last_stack();
#endif
	thread t(TRACE_PARAMS(thread_thread_object_run));
	t.join();
#ifdef STACK_TRACE_FLAG
	CPPUNIT_ASSERT(&s != s1);
	CPPUNIT_ASSERT(&es != es1);
#endif
	CPPUNIT_ASSERT(p != p1);
}

void thread_test_class::thread_join_throw() {
	STACK_TRACE();
	thread_attribute TRACE_CONSTRUCT(attr);
	attr.detached(true);
	thread t(TRACE_PARAMS(thread_run_sleep, attr));
	try{
		t.join();
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == EINVAL);
		#ifdef STACK_TRACE_FLAG
		exception::last_stack().print_stack(stdout);
		#endif
	}
}

namespace{
	class test_obj{
	public:
		static int copy;
		test_obj(){}
		test_obj(const test_obj&){copy++;}
	};
	int test_obj::copy;
	
	class thread_class{
	public:
		static bool thread_started;
		static int int_value;
		static const int *int_ptr;
		static const test_obj *obj_ptr;
		static int copy;
		static void run(){
			thread_started = true;
		}

		static void run_int(int i){
			int_value = i;
		}

		static void run_ref_int(int &i){
			i = 0x1234;
			int_ptr = &i;
		}

		static void run_const_ref_int(const int &i){
			int_ptr = &i;
		}

		static void run_obj(test_obj o){
			obj_ptr = &o;
		}

		static void run_ref_obj(test_obj &o){
			obj_ptr = &o;
		}

		static void run_const_ref_obj(const test_obj &o){
			obj_ptr = &o;
		}

		thread_class(){}

		thread_class(const thread_class&){copy++;}

		void run_type(){
			thread_started = true;
		}

		void run_type() const{
			thread_started = true;
		}

		void run_type_int(int i){
			int_value = i;
		}

		void run_type_int(int i) const{
			int_value = i;
		}

		void run_type_ref_int(int &i){
			i = 0x1234;
			int_ptr = &i;
		}

		void run_type_ref_int(int &i) const{
			i = 0x1234;
			int_ptr = &i;
		}

		void run_type_const_ref_int(const int &i){
			int_ptr = &i;
		}

		void run_type_const_ref_int(const int &i)const {
			int_ptr = &i;
		}

		void run_type_obj(test_obj o){
			obj_ptr = &o;
		}

		void run_type_obj(test_obj o)const{
			obj_ptr = &o;
		}

		void run_type_ref_obj(test_obj &o){
			obj_ptr = &o;
		}

		void run_type_ref_obj(test_obj &o)const{
			obj_ptr = &o;
		}

		void run_type_const_ref_obj(const test_obj &o){
			obj_ptr = &o;
		}

		void run_type_const_ref_obj(const test_obj &o)const{
			obj_ptr = &o;
		}

	};
	bool thread_class::thread_started;
	int thread_class::int_value;
	const int *thread_class::int_ptr;
	const test_obj *thread_class::obj_ptr;
	int thread_class::copy;
}

void thread_test_class::thread_create() {
	thread_attribute TRACE_CONSTRUCT(attr);
	attr.detached(true);
	thread_class::thread_started = false;
	thread thd(TRACE_PARAMS(&thread_class::run, attr));
	usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::thread_started);
}

void thread_test_class::thread_create_join() {
	thread_class::thread_started = false;
	thread thd(TRACE_PARAMS(&thread_class::run));
	thd.join();
	CPPUNIT_ASSERT(thread_class::thread_started);
}

void thread_test_class::thread_create_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread thd(TRACE_PARAMS(&thread_class::run_int, 0x12345678, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::int_value == 0x12345678);
}

void thread_test_class::thread_create_int_join() {
	thread thd(TRACE_PARAMS(&thread_class::run_int, 0x8765));
	thd.join();
	CPPUNIT_ASSERT(thread_class::int_value == 0x8765);
}

void thread_test_class::thread_create_ref_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	int i = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_ref_int, i, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(i == 0x1234);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_ref_int_join() {
	int i = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_ref_int, i));
	thd.join();
	CPPUNIT_ASSERT(i == 0x1234);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_const_ref_int() {
	const int i = 1;
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread thd(TRACE_PARAMS(&thread_class::run_const_ref_int, i, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_const_ref_int_join() {
	const int i = 1;
	thread thd(TRACE_PARAMS(&thread_class::run_const_ref_int, i));
	thd.join();
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	test_obj o;
	thread_class::obj_ptr = NULL;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr != &o);
	CPPUNIT_ASSERT(1 == test_obj::copy);
}

void thread_test_class::thread_create_obj_join() {
	thread_attribute TRACE_CONSTRUCT(attr);
	test_obj o;
	thread_class::obj_ptr = NULL;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr != &o);
	CPPUNIT_ASSERT(1 == test_obj::copy);
}

void thread_test_class::thread_create_ref_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	test_obj o;
	thread_class::obj_ptr = NULL;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_ref_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_ref_obj_join() {
	test_obj o;
	thread_class::obj_ptr = NULL;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_ref_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_const_ref_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const test_obj o;
	thread_class::obj_ptr = NULL;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_const_ref_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_const_ref_obj_join() {
	const test_obj o;
	thread_class::obj_ptr = NULL;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(&thread_class::run_const_ref_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_type() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	thread_class::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(!thread_class::copy);
}

void thread_test_class::thread_create_type_join() {
	thread_class thc;
	thread_class::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type));
	thd.join();
	CPPUNIT_ASSERT(!thread_class::copy);
}

void thread_test_class::thread_create_const_type() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	thread_class::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(!thread_class::copy);
}

void thread_test_class::thread_create_const_type_join() {
	const thread_class thc;
	thread_class::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type));
	thd.join();
	CPPUNIT_ASSERT(!thread_class::copy);

}

void thread_test_class::thread_create_type_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	thread_class::int_value = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_int, 0x1234, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::int_value == 0x1234);
}

void thread_test_class::thread_create_type_int_join() {
	thread_class thc;
	thread_class::int_value = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_int, 0x4321));
	thd.join();
	CPPUNIT_ASSERT(thread_class::int_value == 0x4321);
}

void thread_test_class::thread_create_const_type_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	thread_class::int_value = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_int, 0x5678, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::int_value == 0x5678);
}

void thread_test_class::thread_create_const_type_int_join() {
	const thread_class thc;
	thread_class::int_value = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_int, 0x2345));
	thd.join();
	CPPUNIT_ASSERT(thread_class::int_value == 0x2345);
}

void thread_test_class::thread_create_type_ref_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_int, i, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(0x1234 == i);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_type_ref_int_join() {
	thread_class thc;
	int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_int, i));
	thd.join();
	CPPUNIT_ASSERT(0x1234 == i);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_const_type_ref_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_int, i, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(0x1234 == i);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_const_type_ref_int_join() {
	const thread_class thc;
	int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_int, i));
	thd.join();
	CPPUNIT_ASSERT(0x1234 == i);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_type_const_ref_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	const int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_int, i, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_type_const_ref_int_join() {
	thread_class thc;
	const int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_int, i));
	thd.join();
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_const_type_const_ref_int() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	const int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_int, i, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_const_type_const_ref_int_join() {
	const thread_class thc;
	const int i = 0;
	thread_class::int_ptr = NULL;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_int, i));
	thd.join();
	CPPUNIT_ASSERT(thread_class::int_ptr == &i);
}

void thread_test_class::thread_create_type_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr != &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(test_obj::copy == 1);
}

void thread_test_class::thread_create_type_obj_join() {
	thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr != &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(test_obj::copy == 1);
}

void thread_test_class::thread_create_const_type_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr != &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(test_obj::copy == 1);
}

void thread_test_class::thread_create_const_type_obj_join() {
	const thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr != &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(test_obj::copy == 1);
}

void thread_test_class::thread_create_type_ref_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_type_ref_obj_join() {
	thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);

}

void thread_test_class::thread_create_const_type_ref_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_const_type_ref_obj_join() {
	const thread_class thc;
	test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_ref_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);

}

void thread_test_class::thread_create_type_const_ref_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	const test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_type_const_ref_obj_join() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	thread_class thc;
	const test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_const_type_const_ref_obj() {
	thread_attribute TRACE_CONSTRUCT(attr);
        attr.detached(true);
	const thread_class thc;
	const test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_obj, o, attr));
        usleep(10*1000);
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);
}

void thread_test_class::thread_create_const_type_const_ref_obj_join() {
	const thread_class thc;
	const test_obj o;
	thread_class::obj_ptr = NULL;
	thread_class::copy = 0;
	test_obj::copy = 0;
	thread thd(TRACE_PARAMS(thc, &thread_class::run_type_const_ref_obj, o));
	thd.join();
	CPPUNIT_ASSERT(thread_class::obj_ptr == &o);
	CPPUNIT_ASSERT(!thread_class::copy);
	CPPUNIT_ASSERT(!test_obj::copy);

}

