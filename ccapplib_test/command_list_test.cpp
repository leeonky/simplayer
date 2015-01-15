#include <string>
#include "ccapplib_test/command_list_test.h"
#include "ccapplib/command_list.h"

using namespace ccapplib;

#define TMP_CMDLIST_FILE "/tmp/test.lst"
void command_list_test_class::empty_list(){
	system("echo -n '' > "TMP_CMDLIST_FILE);
	command_list list(TRACE_PARAMS("", TMP_CMDLIST_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)0, list.parameters().size());
}

void command_list_test_class::one_arg_one_cmd(){
	system("echo -n 'aa' > "TMP_CMDLIST_FILE);
	command_list list(TRACE_PARAMS("arg0", TMP_CMDLIST_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)1, list.parameters().size());
	CPPUNIT_ASSERT_EQUAL(2, list.parameters().first()->count());
	CPPUNIT_ASSERT_EQUAL(std::string("arg0"), std::string(list.parameters().first()->arguments()[0]));
	CPPUNIT_ASSERT_EQUAL(std::string("aa"), std::string(list.parameters().first()->arguments()[1]));
}

void command_list_test_class::two_arg_one_cmd(){
	system("echo 'aa' > "TMP_CMDLIST_FILE);
	system("echo -n 'bb' >> "TMP_CMDLIST_FILE);
	command_list list(TRACE_PARAMS("arg0", TMP_CMDLIST_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)1, list.parameters().size());
	CPPUNIT_ASSERT_EQUAL(3, list.parameters().first()->count());
	CPPUNIT_ASSERT_EQUAL(std::string("arg0"), std::string(list.parameters().first()->arguments()[0]));
	CPPUNIT_ASSERT_EQUAL(std::string("aa"), std::string(list.parameters().first()->arguments()[1]));
	CPPUNIT_ASSERT_EQUAL(std::string("bb"), std::string(list.parameters().first()->arguments()[2]));
}

void command_list_test_class::one_arg_one_arg(){
	system("echo 'aa' > "TMP_CMDLIST_FILE);
	system("echo '' >> "TMP_CMDLIST_FILE);
	system("echo -n 'bb' >> "TMP_CMDLIST_FILE);
	command_list list(TRACE_PARAMS("arg0", TMP_CMDLIST_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)2, list.parameters().size());
	CPPUNIT_ASSERT_EQUAL(2, list.parameters().first()->count());
	CPPUNIT_ASSERT_EQUAL(std::string("arg0"), std::string(list.parameters().first()->arguments()[0]));
	CPPUNIT_ASSERT_EQUAL(std::string("aa"), std::string(list.parameters().first()->arguments()[1]));
	CPPUNIT_ASSERT_EQUAL(2, list.parameters().last()->count());
	CPPUNIT_ASSERT_EQUAL(std::string("arg0"), std::string(list.parameters().last()->arguments()[0]));
	CPPUNIT_ASSERT_EQUAL(std::string("bb"), std::string(list.parameters().last()->arguments()[1]));
}

void command_list_test_class::multi_splitline(){
	system("echo 'aa' > "TMP_CMDLIST_FILE);
	system("echo '' >> "TMP_CMDLIST_FILE);
	system("echo '' >> "TMP_CMDLIST_FILE);
	system("echo -n 'bb' >> "TMP_CMDLIST_FILE);
	command_list list(TRACE_PARAMS("arg0", TMP_CMDLIST_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)2, list.parameters().size());
	CPPUNIT_ASSERT_EQUAL(2, list.parameters().first()->count());
	CPPUNIT_ASSERT_EQUAL(std::string("arg0"), std::string(list.parameters().first()->arguments()[0]));
	CPPUNIT_ASSERT_EQUAL(std::string("aa"), std::string(list.parameters().first()->arguments()[1]));
	CPPUNIT_ASSERT_EQUAL(2, list.parameters().last()->count());
	CPPUNIT_ASSERT_EQUAL(std::string("arg0"), std::string(list.parameters().last()->arguments()[0]));
	CPPUNIT_ASSERT_EQUAL(std::string("bb"), std::string(list.parameters().last()->arguments()[1]));
}
