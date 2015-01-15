#ifndef MUTEX_TEST_H_
#define MUTEX_TEST_H_

#include "cctest/util.h"

class mutex_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (mutex_test_class);
	CPPUNIT_TEST (mutex_lock);
	CPPUNIT_TEST (mutex_trylock);
	CPPUNIT_TEST (mutex_guardtrylock);
	CPPUNIT_TEST (mutex_timedlock_block);
	CPPUNIT_TEST (mutex_timedlock_lock);
	CPPUNIT_TEST_SUITE_END();

	void mutex_lock();
	void mutex_trylock();
	void mutex_guardtrylock();
	void mutex_timedlock_block();
	void mutex_timedlock_lock();
};


#endif //MUTEX_TEST_H_
