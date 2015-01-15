#ifndef CCAPPLIB_TEST_COMMAND_LIST_TEST_TEST
#define CCAPPLIB_TEST_COMMAND_LIST_TEST_TEST

#include "cctest/util.h"

class command_list_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(command_list_test_class);
	CPPUNIT_TEST(empty_list);
	CPPUNIT_TEST(one_arg_one_cmd);
	CPPUNIT_TEST(two_arg_one_cmd);
	CPPUNIT_TEST(one_arg_one_arg);
	CPPUNIT_TEST(multi_splitline);
	CPPUNIT_TEST_SUITE_END();
	
	void empty_list();
	void one_arg_one_cmd();
	void two_arg_one_cmd();
	void one_arg_one_arg();
	void multi_splitline();
};

#endif //CCAPPLIB_TEST_COMMAND_LIST_TEST_TEST
