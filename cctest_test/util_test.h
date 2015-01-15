#ifndef UTIL_TEST_H_
#define UTIL_TEST_H_

#include "cctest/util.h"

class util_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (util_test_class);
	CPPUNIT_TEST(interface_testor_test);
	CPPUNIT_TEST_SUITE_END();

	void interface_testor_test();
};

#endif // UTIL_TEST_H_
