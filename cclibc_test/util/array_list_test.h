#ifndef UTIL_ARRAY_LIST_TEST_H_
#define UTIL_ARRAY_LIST_TEST_H_

#include "cctest/util.h"

class array_list_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (array_list_test_class);
	CPPUNIT_TEST(array_list_empty);
	CPPUNIT_TEST(array_list_append_first);
	CPPUNIT_TEST(array_list_append_realloc);
	CPPUNIT_TEST(array_list_append_realloc_cap);
	CPPUNIT_TEST(array_list_clear);
	CPPUNIT_TEST(array_list_clear_deconstruc);
	CPPUNIT_TEST(array_list_deconstruct);
	CPPUNIT_TEST_SUITE_END();

	void array_list_empty();
	void array_list_append_first();
	void array_list_append_realloc();
	void array_list_append_realloc_cap();
	void array_list_clear();
	void array_list_clear_deconstruc();
	void array_list_deconstruct();
};

#endif //UTIL_ARRAY_LIST_TEST_H_

