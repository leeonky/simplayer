#ifndef UTIL_AVL_TREE_TEST_H_
#define UTIL_AVL_TREE_TEST_H_

#include "cctest/util.h"

class avl_tree_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(avl_tree_test_class);
	CPPUNIT_TEST(node_properties);
	CPPUNIT_TEST(node_balance_1_2);
	CPPUNIT_TEST(node_balance_1_1_2);
	CPPUNIT_TEST(node_balance_n1_2);
	CPPUNIT_TEST(node_balance_n1_1_2);
	CPPUNIT_TEST(node_balance_n1_1lr_2);
	CPPUNIT_TEST(node_balance_n1_n1_2);
	CPPUNIT_TEST(node_balance_n1_n1lr_2);

	CPPUNIT_TEST(node_balance_n2_n1);
	CPPUNIT_TEST(node_balance_n2_n1_n1);
	CPPUNIT_TEST(node_balance_n2_1);
	CPPUNIT_TEST(node_balance_n2_1_1);
	CPPUNIT_TEST(node_balance_n2_1lr_1);
	CPPUNIT_TEST(node_balance_n2_n1_1);
	CPPUNIT_TEST(node_balance_n2_n1lr_1);
	CPPUNIT_TEST(node_balance_noroot_2);
	CPPUNIT_TEST(node_balance_noroot_n2);

	CPPUNIT_TEST(compare_value);
	CPPUNIT_TEST(compare_string);

	CPPUNIT_TEST(avl_tree_empty);
	CPPUNIT_TEST(avl_tree_add_to_empty);
	CPPUNIT_TEST(avl_tree_add_less);
	CPPUNIT_TEST(avl_tree_add_more);
	CPPUNIT_TEST(avl_tree_add_less_more);
	CPPUNIT_TEST(avl_tree_add_1_2);
	CPPUNIT_TEST(avl_tree_add_n1_2);
	CPPUNIT_TEST(avl_tree_add_n2_n1);
	CPPUNIT_TEST(avl_tree_add_n2_1);
	
	CPPUNIT_TEST(avl_tree_find_only);
	CPPUNIT_TEST(avl_tree_find_equal);
	CPPUNIT_TEST(avl_tree_find_less);
	CPPUNIT_TEST(avl_tree_find_more);
	
	CPPUNIT_TEST(avl_tree_add_nodes);
	CPPUNIT_TEST(avl_tree_find);
	
	CPPUNIT_TEST(avl_root_swap_node_left1);
	CPPUNIT_TEST(avl_root_swap_node_left2);
	CPPUNIT_TEST(avl_root_swap_node_left3);
	CPPUNIT_TEST(avl_root_swap_node_left4);
	CPPUNIT_TEST(avl_root_swap_node_left_son1);
	CPPUNIT_TEST(avl_root_swap_node_left_son2);
	CPPUNIT_TEST(avl_root_swap_node_left_son3);
	CPPUNIT_TEST(avl_root_swap_node_left_son4);
	CPPUNIT_TEST(avl_root_swap_node_left_son5);
	CPPUNIT_TEST(avl_root_swap_node_left_son6);
	
	CPPUNIT_TEST(avl_root_swap_node_right1);
	CPPUNIT_TEST(avl_root_swap_node_right2);
	CPPUNIT_TEST(avl_root_swap_node_right3);
	CPPUNIT_TEST(avl_root_swap_node_right4);
	CPPUNIT_TEST(avl_root_swap_node_right_son1);
	CPPUNIT_TEST(avl_root_swap_node_right_son2);
	CPPUNIT_TEST(avl_root_swap_node_right_son3);
	CPPUNIT_TEST(avl_root_swap_node_right_son4);
	CPPUNIT_TEST(avl_root_swap_node_right_son5);
	CPPUNIT_TEST(avl_root_swap_node_right_son6);
	
	CPPUNIT_TEST(avl_tree_remove_root_only);
	CPPUNIT_TEST(avl_tree_remove_left_last_node);
	CPPUNIT_TEST(avl_tree_remove_right_last_node);
	CPPUNIT_TEST(avl_tree_remove_left_last_node_bl);
	CPPUNIT_TEST(avl_tree_remove_right_last_node_bl);
	CPPUNIT_TEST(avl_tree_remove_left_swap);
	CPPUNIT_TEST(avl_tree_remove_right_swap);
	CPPUNIT_TEST(avl_tree_remove_root_middle);
	CPPUNIT_TEST(avl_tree_remove_root_left);
	CPPUNIT_TEST(avl_tree_remove_root_right);
	CPPUNIT_TEST(avl_tree_remove_nodes);
	CPPUNIT_TEST(avl_tree_clear);
	CPPUNIT_TEST(avl_tree_discard);

	CPPUNIT_TEST_SUITE_END();

	void node_properties();

	void node_balance_1_2();
	void node_balance_1_1_2();
	void node_balance_n1_2();
	void node_balance_n1_1_2();
	void node_balance_n1_1lr_2();
	void node_balance_n1_n1_2();
	void node_balance_n1_n1lr_2();

	void node_balance_n2_n1lr_1();
	void node_balance_n2_n1();
	void node_balance_n2_n1_n1();
	void node_balance_n2_1();
	void node_balance_n2_1_1();
	void node_balance_n2_1lr_1();
	void node_balance_n2_n1_1();
	void node_balance_noroot_2();
	void node_balance_noroot_n2();

	void compare_value();
	void compare_string();

	void avl_tree_empty();
	void avl_tree_add_to_empty();
	void avl_tree_add_less();
	void avl_tree_add_more();
	void avl_tree_add_less_more();
	void avl_tree_add_1_2();
	void avl_tree_add_n1_2();
	void avl_tree_add_n2_n1();
	void avl_tree_add_n2_1();
	
	void avl_tree_find_only();
	void avl_tree_find_equal();
	void avl_tree_find_less();
	void avl_tree_find_more();
	
	void avl_tree_add_nodes();
	void avl_tree_find();
	
	void avl_root_swap_node_left1();
	void avl_root_swap_node_left2();
	void avl_root_swap_node_left3();
	void avl_root_swap_node_left4();
	void avl_root_swap_node_left_son1();
	void avl_root_swap_node_left_son2();
	void avl_root_swap_node_left_son3();
	void avl_root_swap_node_left_son4();
	void avl_root_swap_node_left_son5();
	void avl_root_swap_node_left_son6();
	
	void avl_root_swap_node_right1();
	void avl_root_swap_node_right2();
	void avl_root_swap_node_right3();
	void avl_root_swap_node_right4();
	void avl_root_swap_node_right_son1();
	void avl_root_swap_node_right_son2();
	void avl_root_swap_node_right_son3();
	void avl_root_swap_node_right_son4();
	void avl_root_swap_node_right_son5();
	void avl_root_swap_node_right_son6();

	void avl_tree_remove_root_only();
	void avl_tree_remove_left_last_node();
	void avl_tree_remove_right_last_node();
	void avl_tree_remove_left_last_node_bl();
	void avl_tree_remove_right_last_node_bl();
	void avl_tree_remove_left_swap();
	void avl_tree_remove_right_swap();
	void avl_tree_remove_root_middle();
	void avl_tree_remove_root_left();
	void avl_tree_remove_root_right();
	
	void avl_tree_remove_nodes();
	void avl_tree_clear();
	void avl_tree_discard();
};

#endif //UTIL_AVL_TREE_TEST_H_
