#ifndef CONDITION_TEST_H_
#define CONDITION_TEST_H_

#include "cctest/util.h"

class condition_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE (condition_test_class);
	CPPUNIT_TEST (condition_wait);
	CPPUNIT_TEST (condition_timedwait);
	CPPUNIT_TEST (condition_broadcast);
	CPPUNIT_TEST_SUITE_END();
	void condition_wait();
	void condition_timedwait();
	void condition_broadcast();
};

#endif //CONDITION_TEST_H_

