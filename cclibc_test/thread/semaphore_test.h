#ifndef SEMAPHORE_TEST_H_
#define SEMAPHORE_TEST_H_

#include "cctest/util.h"

class semaphore_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE (semaphore_test_class);
	CPPUNIT_TEST (semaphore_wait);
	CPPUNIT_TEST (semaphore_trywait);
	CPPUNIT_TEST (semaphore_timedwait);
	CPPUNIT_TEST_SUITE_END();
	void semaphore_wait();
	void semaphore_trywait();
	void semaphore_timedwait();
};

#endif //SEMAPHORE_TEST_H_
