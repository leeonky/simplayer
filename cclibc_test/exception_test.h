#ifndef EXCEPTION_TEST_H_
#define EXCEPTION_TEST_H_

#include "cctest/util.h"

class exception_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (exception_test_class);
	CPPUNIT_TEST(exception_properties);
	CPPUNIT_TEST(exception_message);
	CPPUNIT_TEST(exception_trace_1);
	CPPUNIT_TEST(exception_trace_2);
	CPPUNIT_TEST(exception_setins);
	CPPUNIT_TEST(exception_throw_new_0);
	CPPUNIT_TEST(exception_throw_0);
	CPPUNIT_TEST(exception_throw_1);
	CPPUNIT_TEST(exception_throw_new_1);
	CPPUNIT_TEST(exception_throw_clear);
	CPPUNIT_TEST(exception_throw_new_clear);
	CPPUNIT_TEST(posix_error_properties);
	CPPUNIT_TEST(illegal_argument_properties);
	CPPUNIT_TEST(exception_destroy);
	CPPUNIT_TEST(exception_destroy1);
	CPPUNIT_TEST_SUITE_END();

	void exception_properties();
	void exception_message();
	void exception_trace_1();
	void exception_trace_2();
	void exception_setins();
	void exception_throw_new_0();
	void exception_throw_0();
	void exception_throw_1();
	void exception_throw_new_1();
	void exception_throw_clear();
	void exception_throw_new_clear();
	void posix_error_properties();
	void illegal_argument_properties();
	void exception_destroy();
	void exception_destroy1();
};

#endif

