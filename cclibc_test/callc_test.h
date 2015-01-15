#ifndef CALLC_TEST_H_
#define CALLC_TEST_H_

#include "cctest/util.h"

class callc_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (callc_test_class);
	CPPUNIT_TEST (return_error_1);
	CPPUNIT_TEST (return_error_2);
	CPPUNIT_TEST (return_error_3);
	CPPUNIT_TEST (set_errno_1);
	CPPUNIT_TEST (set_errno_2);
	CPPUNIT_TEST (set_errno_3);
	CPPUNIT_TEST_SUITE_END();

	void return_error_1();
	void return_error_2();
	void return_error_3();

	void set_errno_1();
	void set_errno_2();
	void set_errno_3();
};

#endif //CALLC_TEST_H_
