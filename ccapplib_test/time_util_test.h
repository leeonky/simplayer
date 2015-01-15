#ifndef CCAPPLIB_TEST_TIME_UTIL_TEST
#define CCAPPLIB_TEST_TIME_UTIL_TEST

#include "cctest/util.h"

class time_util_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(time_util_test_class);
	CPPUNIT_TEST(time_bucket_create);
	CPPUNIT_TEST(time_bucket_contains);
	CPPUNIT_TEST_SUITE_END();
	void time_bucket_create();
	void time_bucket_contains();
};

#endif //CCAPPLIB_TEST_TIME_UTIL_TEST
