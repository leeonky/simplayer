#ifndef ENV_TEST_H_
#define ENV_TEST_H_

#include "cctest/util.h"

class env_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (env_test_class);
	CPPUNIT_TEST(algin_test);
	CPPUNIT_TEST(algin_macro_test);
	CPPUNIT_TEST_SUITE_END();

	void algin_test();
	void algin_macro_test();
};

#endif // ENV_TEST_H_
