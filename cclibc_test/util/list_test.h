#ifndef UTIL_LIST_TEST_H_
#define UTIL_LIST_TEST_H_

#include "cctest/util.h"

class list_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (list_test_class);
	CPPUNIT_TEST(empty_node);
	CPPUNIT_TEST(empty_list);
	CPPUNIT_TEST(add_to_empty);
	CPPUNIT_TEST(add_to_nonempty);
	CPPUNIT_TEST(remove_only);
	CPPUNIT_TEST(remove__first);
	CPPUNIT_TEST(remove_middle);
	CPPUNIT_TEST(remove_last);
	CPPUNIT_TEST(insert_before_empty);
	CPPUNIT_TEST(insert_before_null);
	CPPUNIT_TEST(insert_before_first);
	CPPUNIT_TEST(insert_before_mid);
	CPPUNIT_TEST(insert_after_empty);
	CPPUNIT_TEST(insert_after_null);
	CPPUNIT_TEST(insert_after_last);
	CPPUNIT_TEST(insert_after_mid);
	CPPUNIT_TEST(list_foreach);
	CPPUNIT_TEST(list_r_foreach);
	CPPUNIT_TEST(list_clear);
	CPPUNIT_TEST(list_discard);
	CPPUNIT_TEST(s_empty_node);
	CPPUNIT_TEST(s_empty_list);
	CPPUNIT_TEST(s_add_to_empty);
	CPPUNIT_TEST(s_add_to_list);
	CPPUNIT_TEST(s_remove_only);
	CPPUNIT_TEST(s_remove_first);
	CPPUNIT_TEST(s_remove_other);
	CPPUNIT_TEST(s_remove_last);
	CPPUNIT_TEST(s_remove_first_only);
	CPPUNIT_TEST(s_remove_first_other);
	CPPUNIT_TEST(s_list_foreach);
	CPPUNIT_TEST(s_remove2_only);
	CPPUNIT_TEST(s_remove2_first);
	CPPUNIT_TEST(s_remove2_other);
	CPPUNIT_TEST(s_remove2_last);
	CPPUNIT_TEST(replace_only);
	CPPUNIT_TEST(replace_other);
	CPPUNIT_TEST(s_foreach2);
	CPPUNIT_TEST(s_list_clear);
	CPPUNIT_TEST(slist_discard);
	CPPUNIT_TEST(list_c_foreach);
	CPPUNIT_TEST(list_c_rforeach);
	CPPUNIT_TEST(slist_c_foreach);
	CPPUNIT_TEST_SUITE_END();

	void empty_node();
	void empty_list();
	void add_to_empty();
	void add_to_nonempty();
	void remove_only();
	void remove__first();
	void remove_middle();
	void remove_last();
	void insert_before_empty();
	void insert_before_null();
	void insert_before_first();
	void insert_before_mid();
	void list_foreach();
	void list_r_foreach();
	void insert_after_empty();
	void insert_after_null();
	void insert_after_last();
	void insert_after_mid();
	void list_clear();
	void list_discard();
	void s_empty_node();
	void s_empty_list();
	void s_add_to_empty();
	void s_add_to_list();
	void s_remove_only();
	void s_remove_first();
	void s_remove_other();
	void s_remove_last();
	void s_remove_first_only();
	void s_remove_first_other();
	void s_list_foreach();
	void s_remove2_only();
	void s_remove2_first();
	void s_remove2_other();
	void s_remove2_last();
	void s_foreach2();
	void replace_only();
	void replace_other();
	void s_list_clear();
	void slist_discard();
	void list_c_foreach();
	void list_c_rforeach();
	void slist_c_foreach();
};



#endif //UTIL_LIST_TEST_H_
