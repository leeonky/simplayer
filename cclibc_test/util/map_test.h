#ifndef UTIL_MAP_TEST_H_
#define UTIL_MAP_TEST_H_

#include "cctest/util.h"

class map_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (map_test_class);
	CPPUNIT_TEST(map_add);
	CPPUNIT_TEST(map_add_fun);
	CPPUNIT_TEST(map_default);
	CPPUNIT_TEST(map_contains);
	CPPUNIT_TEST(map_clear);
	CPPUNIT_TEST(map_des);
	CPPUNIT_TEST(map_foreach);
	CPPUNIT_TEST(map_c_foreach);
	CPPUNIT_TEST(map_rforeach);
	CPPUNIT_TEST(map_c_rforeach);
	CPPUNIT_TEST(map_remove);
	CPPUNIT_TEST(map_remove_exp);
	CPPUNIT_TEST(map_erase);
	CPPUNIT_TEST_SUITE_END();
	void map_add();
	void map_default();
	void map_add_fun();
	void map_contains();
	void map_clear();
	void map_des();
	void map_foreach();
	void map_c_foreach();
	void map_rforeach();
	void map_c_rforeach();
	void map_remove();
	void map_remove_exp();
	void map_erase();
};



#endif //UTIL_MAP_TEST_H_
