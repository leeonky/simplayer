#ifndef TRACE_TEST_H_
#define TRACE_TEST_H_

#include "cctest/util.h"

class trace_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (trace_test_class);
	CPPUNIT_TEST(stack_recorder_properties);
	CPPUNIT_TEST(stack_properties);
	CPPUNIT_TEST(stack_trace);
	CPPUNIT_TEST(stack_copy);
	CPPUNIT_TEST(stack_static);
	CPPUNIT_TEST(stack_trace_guard);
	CPPUNIT_TEST(stack_trace_trace_macro);
	CPPUNIT_TEST_SUITE_END();

	void stack_recorder_properties();
	void stack_properties();
	void stack_trace();
	void stack_copy();
	void stack_static();
	void stack_trace_guard();
	void stack_trace_trace_macro();
};

#endif
