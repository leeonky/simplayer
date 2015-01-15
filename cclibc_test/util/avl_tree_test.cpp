#include "cclibc_test/util/avl_tree_test.h"
#include "cclibc/util/avl_tree.h"

using CCLIBC::avl_tree;
using CCLIBC::comparer;

typedef avl_tree<const char*, const char*> avtree;

#define CHECK_NODE(n,l,r,f,b) do{\
CPPUNIT_ASSERT(n.left() == l);\
CPPUNIT_ASSERT(n.right() == r);\
CPPUNIT_ASSERT(n.father() == f);\
CPPUNIT_ASSERT(n.balance_factor() == b);}while(0)

#define CHECK_LAST_NODE(n, f)	CHECK_NODE(n, NULL, NULL, f, 0)

#define SET_NODE(n,l,r,f,b) do{\
n._left = l;n._right = r; n._father =f; n._bf = b;}while(0)

void avl_tree_test_class::node_properties(){
	const char *str1 = "123";
	const char *str2 = "456";
	avtree::node n(str1,str2);
	CPPUNIT_ASSERT(n.value() == str1);
	CPPUNIT_ASSERT(n.payload() == str2);
	CPPUNIT_ASSERT(!n.left());
	CPPUNIT_ASSERT(!n.right());
	CPPUNIT_ASSERT(!n.father());
	CPPUNIT_ASSERT(!n.balance_factor());

	avtree::node n1(str1,str2);
	avtree::node n2(str1,str2);
	avtree::node n3(str1,str2);
	n._left = &n1;
	n._right= &n2;
	n._father = &n3;
	n._bf = 1;
	CPPUNIT_ASSERT(n.left() == &n1);
	CPPUNIT_ASSERT(n.right() == &n2);
	CPPUNIT_ASSERT(n.father() == &n3);
	CPPUNIT_ASSERT(1 == n.balance_factor());
}
void avl_tree_test_class::node_balance_1_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, NULL, NULL, 2);
	
	avtree tree;
	avtree::node *n = tree.update_balance_2(&n3);
	CPPUNIT_ASSERT(n == &n2);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::node_balance_1_1_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, &n4, &n5, 1);
	SET_NODE(n4, NULL, NULL, &n3, 0);
	SET_NODE(n5, &n3, &n6, NULL, 2);
	SET_NODE(n6, NULL, NULL, &n5, 0);

	avtree tree;
	avtree::node *n = tree.update_balance_2(&n5);
	CPPUNIT_ASSERT(n == &n3);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, NULL, &n3, 1);
	CHECK_NODE(n3, &n2, &n5, NULL, 0);
	CHECK_LAST_NODE(n4, &n5);
	CHECK_NODE(n5, &n4, &n6, &n3, 0);
	CHECK_LAST_NODE(n6, &n5);
}

void avl_tree_test_class::node_balance_n1_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL);
	SET_NODE(n1, NULL, &n2, &n3, -1);
	SET_NODE(n2, NULL, NULL, &n1, 0);
	SET_NODE(n3, &n1, NULL, NULL, 2);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_2(&n3) == &n2);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::node_balance_n1_1_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, &n4, &n5, -1);
	SET_NODE(n3, NULL, NULL, &n4, 0);
	SET_NODE(n4, &n3, NULL, &n2, 1);
	SET_NODE(n5, &n2, &n6, NULL, 2);
	SET_NODE(n6, NULL, NULL, &n5, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_2(&n5) == &n4);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, &n4, 0);
	CHECK_LAST_NODE(n3, &n2);
	CHECK_NODE(n4, &n2, &n5, NULL, 0);
	CHECK_NODE(n5, NULL, &n6, &n4, -1);
}

void avl_tree_test_class::node_balance_n1_1lr_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL), n7(NULL, NULL), n8(NULL, NULL), n9(NULL, NULL), n10(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, &n6, &n8, -1);
	SET_NODE(n4, NULL, NULL, &n5, 0);
	SET_NODE(n5, &n4, NULL, &n6, 1);
	SET_NODE(n6, &n5, &n7, &n3, 1);
	SET_NODE(n7, NULL, NULL, &n6, 0);
	SET_NODE(n8, &n3, &n9, NULL, 2);
	SET_NODE(n9, NULL, &n10, &n8, -1);
	SET_NODE(n10, NULL, NULL, &n9, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_2(&n8) == &n6);
	CHECK_NODE(n3, &n2, &n5, &n6, 0);
	CHECK_NODE(n5, &n4, NULL, &n3, 1);
	CHECK_NODE(n6, &n3, &n8, NULL, 0);
	CHECK_LAST_NODE(n7, &n8);
	CHECK_NODE(n8, &n7, &n9, &n6, -1);
}

void avl_tree_test_class::node_balance_n1_n1_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, &n3, &n5, -1);
	SET_NODE(n3, NULL, &n4, &n2, -1);
	SET_NODE(n4, NULL, NULL, &n3, 0);
	SET_NODE(n5, &n2, &n6, NULL, 2);
	SET_NODE(n6, NULL, NULL, &n5, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_2(&n5) == &n3);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, NULL, &n3, 1);
	CHECK_NODE(n3, &n2, &n5, NULL, 0);
	CHECK_LAST_NODE(n4, &n5);
	CHECK_NODE(n5, &n4, &n6, &n3, 0);
	CHECK_LAST_NODE(n6, &n5);
}

void avl_tree_test_class::node_balance_n1_n1lr_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL), n7(NULL, NULL), n8(NULL, NULL), n9(NULL, NULL), n10(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, &n5, &n8, -1);
	SET_NODE(n4, NULL, NULL, &n5, 0);
	SET_NODE(n5, &n4, &n6, &n3, -1);
	SET_NODE(n6, NULL, &n7, &n5, -1);
	SET_NODE(n7, NULL, NULL, &n6, 0);
	SET_NODE(n8, &n3, &n9, NULL, 2);
	SET_NODE(n9, NULL, &n10, &n8, -1);
	SET_NODE(n10, NULL, NULL, &n9, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_2(&n8) == &n5);
	CHECK_NODE(n3, &n2, &n4, &n5, 1);
	CHECK_LAST_NODE(n4, &n3);
	CHECK_NODE(n5, &n3, &n8, NULL, 0);
	CHECK_NODE(n6, NULL, &n7, &n8, -1);
	CHECK_NODE(n8, &n6, &n9, &n5, 0);
}

void avl_tree_test_class::node_balance_n2_n1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL);
	SET_NODE(n1, NULL, &n2, NULL, -2);
	SET_NODE(n2, NULL, &n3, &n1, -1);
	SET_NODE(n3, NULL, NULL, &n2, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n1) == &n2);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::node_balance_n2_n1_n1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, &n4, NULL, -2);
	SET_NODE(n3, NULL, NULL, &n4, 0);
	SET_NODE(n4, &n3, &n5, &n2, -1);
	SET_NODE(n5, NULL, &n6, &n4, -1);
	SET_NODE(n6, NULL, NULL, &n5, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n2) == &n4);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, &n4, 0);
	CHECK_LAST_NODE(n3, &n2);
	CHECK_NODE(n4, &n2, &n5, NULL, 0);
	CHECK_NODE(n5, NULL, &n6, &n4, -1);
	CHECK_LAST_NODE(n6, &n5);
}

void avl_tree_test_class::node_balance_n2_1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL);
	SET_NODE(n1, NULL, &n3, NULL, -2);
	SET_NODE(n2, NULL, NULL, &n3, 0);
	SET_NODE(n3, &n2, NULL, &n1, 1);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n1) == &n2);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::node_balance_n2_1_1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, &n5, NULL, -2);
	SET_NODE(n3, NULL, NULL, &n4, 0);
	SET_NODE(n4, &n3, NULL, &n5, 1);
	SET_NODE(n5, &n4, &n6, &n2, 1);
	SET_NODE(n6, NULL, NULL, &n5, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n2) == &n4);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, &n4, 0);
	CHECK_LAST_NODE(n3, &n2);
	CHECK_NODE(n4, &n2, &n5, NULL, 0);
	CHECK_NODE(n5, NULL, &n6, &n4, -1);
	CHECK_LAST_NODE(n6, &n5);
}

void avl_tree_test_class::node_balance_n2_1lr_1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL), n7(NULL, NULL), n8(NULL, NULL), n9(NULL, NULL), n10(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, &n8, NULL, -2);
	SET_NODE(n4, NULL, NULL, &n5, 0);
	SET_NODE(n5, &n4, NULL, &n6, 1);
	SET_NODE(n6, &n5, &n7, &n8, 1);
	SET_NODE(n7, NULL, NULL, &n6, 0);
	SET_NODE(n8, &n6, &n9, &n3, 1);
	SET_NODE(n9, NULL, &n10, &n8, -1);
	SET_NODE(n10, NULL, NULL, &n9, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n3) == &n6);
	CHECK_NODE(n3, &n2, &n5, &n6, 0);
	CHECK_NODE(n5, &n4, NULL, &n3, 1);
	CHECK_NODE(n6, &n3, &n8, NULL, 0);
	CHECK_LAST_NODE(n7, &n8);
	CHECK_NODE(n8, &n7, &n9, &n6, -1);
}

void avl_tree_test_class::node_balance_n2_n1_1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, &n5, NULL, -2);
	SET_NODE(n3, NULL, &n4, &n5, -1);
	SET_NODE(n4, NULL, NULL, &n3, 0);
	SET_NODE(n5, &n3, &n6, &n2, 1);
	SET_NODE(n6, NULL, NULL, &n5, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n2) == &n3);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, NULL, &n3, 1);
	CHECK_NODE(n3, &n2, &n5, NULL, 0);
	CHECK_LAST_NODE(n4, &n5);
	CHECK_NODE(n5, &n4, &n6, &n3, 0);
	CHECK_LAST_NODE(n6, &n5);
}

void avl_tree_test_class::node_balance_n2_n1lr_1(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL), n6(NULL, NULL), n7(NULL, NULL), n8(NULL, NULL), n9(NULL, NULL), n10(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, &n8, NULL, -2);
	SET_NODE(n4, NULL, NULL, &n5, 0);
	SET_NODE(n5, &n4, &n6, &n8, -1);
	SET_NODE(n6, NULL, &n7, &n5, -1);
	SET_NODE(n7, NULL, NULL, &n6, 0);
	SET_NODE(n8, &n5, &n9, &n3, 1);
	SET_NODE(n9, NULL, &n10, &n8, -1);
	SET_NODE(n10, NULL, NULL, &n9, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n3) == &n5);
	CHECK_NODE(n3, &n2, &n4, &n5, 1);
	CHECK_LAST_NODE(n4, &n3);
	CHECK_NODE(n5, &n3, &n8, NULL, 0);
	CHECK_NODE(n6, NULL, &n7, &n8, -1);
	CHECK_NODE(n8, &n6, &n9, &n5, 0);
}

void avl_tree_test_class::node_balance_noroot_2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, NULL, &n3, 1);
	SET_NODE(n3, &n2, NULL, &n4, 2);
	SET_NODE(n4, &n3, &n5, NULL, 1);
	SET_NODE(n5, NULL, NULL, &n4, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_2(&n3) == &n2);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, &n4, 0);
	CHECK_LAST_NODE(n3, &n2);
	CHECK_NODE(n4, &n2, &n5, NULL, 1);
	CHECK_LAST_NODE(n5, &n4);
}

void avl_tree_test_class::node_balance_noroot_n2(){
	avtree::node n1(NULL, NULL), n2(NULL, NULL),n3(NULL, NULL), n4(NULL, NULL), n5(NULL, NULL);
	SET_NODE(n1, NULL, NULL, &n2, 0);
	SET_NODE(n2, &n1, &n3, NULL, -1);
	SET_NODE(n3, NULL, &n4, &n2, -2);
	SET_NODE(n4, NULL, &n5, &n3, -1);
	SET_NODE(n5, NULL, NULL, &n4, 0);

	avtree tree;
	CPPUNIT_ASSERT(tree.update_balance_n2(&n3) == &n4);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n4, NULL, -1);
	CHECK_LAST_NODE(n3, &n4);
	CHECK_NODE(n4, &n3, &n5, &n2, 0);
	CHECK_LAST_NODE(n5, &n4);
}

void avl_tree_test_class::compare_value(){
	comparer<int> c;
	CPPUNIT_ASSERT(c.compare(1,2) < 0);
	CPPUNIT_ASSERT(!c.compare(1,1));
	CPPUNIT_ASSERT(c.compare(2, 1) > 0);
}

void avl_tree_test_class::compare_string() {
	comparer<char*> c;
	CPPUNIT_ASSERT(c.compare("1", "2") < 0);
	CPPUNIT_ASSERT(!c.compare("1", "1"));
	CPPUNIT_ASSERT(c.compare("4", "3") > 0);
}

typedef avl_tree<int,int> int_tree;

void dumptree(int_tree &tree){
	LIST_FOREACH(int_tree::node, n, tree){
		printf("  [%d--",n->value());
		if(n->left()){
			printf(" L:%d",n->left()->value());
		}
		if(n->father()){
			printf(" F:%d",n->father()->value());
		}
		printf(" BF:%d",n->balance_factor());
		if(n->right()){
			printf(" R:%d",n->right()->value());
		}
		printf("]");
	}
	printf("\n");
}

void avl_tree_test_class::avl_tree_empty(){
	int_tree tree;
	CPPUNIT_ASSERT(!tree.size());
	CPPUNIT_ASSERT(!tree.first());
	CPPUNIT_ASSERT(!tree.last());
	CPPUNIT_ASSERT(!tree.root());
}

void avl_tree_test_class::avl_tree_add_to_empty(){
	int_tree tree;
	int_tree::node n(0,0);

	tree.add(NULL, 0, &n);
	CPPUNIT_ASSERT(tree.first() == &n);
	CPPUNIT_ASSERT(tree.size() == 1);
	CPPUNIT_ASSERT(tree.root() == &n);
	CHECK_LAST_NODE(n, NULL);
}

#define TREE_ADD(tree, fn, n)	tree.add(&fn, tree._cmp.compare(n.value(), fn.value()), &n)

void avl_tree_test_class::avl_tree_add_less(){
	int_tree tree;
	int_tree::node n2(0,0), n1(-1,-1);
	tree.add(NULL, 0, &n2);
	TREE_ADD(tree, n2, n1);
	
	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.last() == &n2);
	CPPUNIT_ASSERT(tree.size() == 2);
	CPPUNIT_ASSERT(tree.root() == &n2);

	CHECK_NODE(n2, &n1, NULL, NULL, 1);
	CHECK_LAST_NODE(n1, &n2);
}

void avl_tree_test_class::avl_tree_add_more(){
	int_tree tree;
	int_tree::node n1(0,0),n2(1,1);
	tree.add(NULL, 0, &n1);
	TREE_ADD(tree, n1, n2);

	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.last() == &n2);
	CPPUNIT_ASSERT(tree.size() == 2);
	CPPUNIT_ASSERT(tree.root() == &n1);

	CHECK_NODE(n1, NULL, &n2, NULL, -1);
	CHECK_LAST_NODE(n2, &n1);
}

void avl_tree_test_class::avl_tree_add_less_more(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3);
	tree.add(NULL, 0, &n2);
	TREE_ADD(tree, n2, n1);
	TREE_ADD(tree, n2, n3);

	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.last() == &n3);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);

}

void avl_tree_test_class::avl_tree_add_1_2(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3);
	tree.add(NULL, 0, &n3);
	TREE_ADD(tree, n3, n2);
	TREE_ADD(tree, n2, n1);

	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.last() == &n3);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::avl_tree_add_n1_2(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3);
	tree.add(NULL, 0, &n3);
	TREE_ADD(tree, n3, n1);
	TREE_ADD(tree, n1, n2);

	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.last() == &n3);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::avl_tree_add_n2_n1(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3);
	tree.add(NULL, 0, &n1);
	TREE_ADD(tree, n1, n2);
	TREE_ADD(tree, n2, n3);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::avl_tree_add_n2_1(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3);
	tree.add(NULL, 0, &n1);
	TREE_ADD(tree, n1, n3);
	TREE_ADD(tree, n3, n2);

	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);
}

void avl_tree_test_class::avl_tree_find_only(){
	int_tree tree;
	int_tree::node n(1,1), *pn;
	tree.add(NULL, 0, &n);
	
	CPPUNIT_ASSERT(!tree.find(pn, n.value()));
	CPPUNIT_ASSERT(pn == &n);
	
	CPPUNIT_ASSERT(tree.find(pn, 0) < 0);
	CPPUNIT_ASSERT(pn == &n);
	
	CPPUNIT_ASSERT(tree.find(pn, 2) > 0);
	CPPUNIT_ASSERT(pn == &n);
}

void avl_tree_test_class::avl_tree_find_equal(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3), *pn;
	tree.add(NULL, 0, &n1);
	TREE_ADD(tree, n1, n3);
	TREE_ADD(tree, n3, n2);
	
	CPPUNIT_ASSERT(!tree.find(pn, n2.value()));
	CPPUNIT_ASSERT(pn == &n2);
}

void avl_tree_test_class::avl_tree_find_less(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3), *pn;
	tree.add(NULL, 0, &n1);
	TREE_ADD(tree, n1, n3);
	TREE_ADD(tree, n3, n2);
	
	CPPUNIT_ASSERT(tree.find(pn, 0) < 0);
	CPPUNIT_ASSERT(pn == &n1);
}

void avl_tree_test_class::avl_tree_find_more(){
	int_tree tree;
	int_tree::node n1(1,1), n2(2,2), n3(3,3), *pn;
	tree.add(NULL, 0, &n1);
	TREE_ADD(tree, n1, n3);
	TREE_ADD(tree, n3, n2);
	
	CPPUNIT_ASSERT(tree.find(pn, 4) > 0);
	CPPUNIT_ASSERT(pn == &n3);
}

namespace{

int max_depth(int_tree::node *n){
	if(!n){
		return 0;
	}
	if(n->payload() != -1){
		return n->payload();
	}
	return n->payload() = (std::max(max_depth(n->left()), max_depth(n->right())) + 1);
}

void validation_tree(int_tree &tree){
	int j = 0;
	for(int_tree::node *n = tree.first(); n; n=n->next(), j++){
		n->payload() = -1;
	}
	for(int_tree::node *n = tree.first(); n; n=n->next(), j++){
		if(n->left()){
			CPPUNIT_ASSERT(n->left()->value() < n->value());
		}
		if(n->right()){
			CPPUNIT_ASSERT(n->right()->value() > n->value());
		}
		int l = max_depth(n->left()) - max_depth(n->right());
		CPPUNIT_ASSERT(n->_bf == l);
		CPPUNIT_ASSERT(l<= 1 && l >=-1);
	}
}
}

static int TREE_ADD_COUNT=0x1000;
void avl_tree_test_class::avl_tree_add_nodes(){
	int_tree tree;
	int_tree::node** ns = new int_tree::node*[TREE_ADD_COUNT];
	for(int i=0; i<TREE_ADD_COUNT; i++){
		ns[i] = new int_tree::node(i, i);
	}

	for(int i=0;i<TREE_ADD_COUNT; i++){
		int i1 = rand() & (TREE_ADD_COUNT - 1);
		int i2 = rand() & (TREE_ADD_COUNT - 1);
		std::swap(ns[i1], ns[i2]);
	}

	for(int i=0; i<TREE_ADD_COUNT;i++){
		int_tree::node *f = NULL;
		int c = tree.find(f, ns[i]->value());
		tree.add(f, c, ns[i]);
		validation_tree(tree);
	}
	int j = 0;
	for(int_tree::node *n = tree.first(); n; n=n->next(), j++){
		CPPUNIT_ASSERT(n->value() == j);
	}
	tree.clear();

	for(int i=0; i<TREE_ADD_COUNT; i++){
		delete ns[i];
	}
	delete[] ns;
}

void avl_tree_test_class::avl_tree_find(){
	int_tree tree;
	int_tree::node** ns = new int_tree::node*[TREE_ADD_COUNT];
	for(int i=0; i<TREE_ADD_COUNT; i++){
		ns[i] = new int_tree::node(i, i);
	}

	for(int i=0;i<TREE_ADD_COUNT; i++){
		int i1 = rand() & (TREE_ADD_COUNT - 1);
		int i2 = rand() & (TREE_ADD_COUNT - 1);
		std::swap(ns[i1], ns[i2]);
	}

	for(int i=0; i<TREE_ADD_COUNT;i++){
		int_tree::node *f = NULL;
		int c = tree.find(f, ns[i]->value());
		tree.add(f, c, ns[i]);
	}

	for(int j=0; j<TREE_ADD_COUNT; j++){
		int_tree::node *n = NULL;
		CPPUNIT_ASSERT(!tree.find(n, j));
		CPPUNIT_ASSERT(n);
		CPPUNIT_ASSERT(n->payload() == j);
	}
	tree.clear();
	for(int i=0; i<TREE_ADD_COUNT; i++){
		delete ns[i];
	}
	delete[] ns;
}

namespace{

void add_tree_node(int_tree &tree, int_tree::node *n){
	int_tree::node *f = NULL;
	int c = tree.find(f, n->value());
	tree.add(f, c, n);
}

}
	
void avl_tree_test_class::avl_root_swap_node_left1(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4), n5(5, 5), n6(6, 6), n7(7, 7), n8(8, 8);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n6, &n5);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, &n4, 0);
	CHECK_LAST_NODE(n3, &n2);
	CHECK_NODE(n4, &n2, &n6, &n5, 1);
	CHECK_NODE(n5, &n4, &n7, NULL, 1);
	CHECK_LAST_NODE(n6, &n4);
	CHECK_NODE(n7, NULL, &n8, &n5, -1);
	CHECK_LAST_NODE(n8, &n7);
	CPPUNIT_ASSERT(tree.root() == &n5);
}

void avl_tree_test_class::avl_root_swap_node_left2(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4), n5(5, 5), n6(6, 6), n7(7, 7), n8(8, 8);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n4, &n3);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n4, &n3, 0);
	CHECK_NODE(n3, &n2, &n5, &n6, 1);
	CHECK_LAST_NODE(n4, &n2);
	CHECK_LAST_NODE(n5, &n3);
	CHECK_NODE(n6, &n3, &n7, NULL, 1);
	CHECK_NODE(n7, NULL, &n8, &n6, -1);
	CHECK_LAST_NODE(n8, &n7);
}

void avl_tree_test_class::avl_root_swap_node_left3(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4), n5(5, 5), n6(6, 6), n7(7, 7), n8(8, 8), n9(9, 9), n10(10, 10), n11(11, 11), n12(12, 12);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n10);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n9);
	add_tree_node(tree, &n11);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n12);
	add_tree_node(tree, &n3);
	tree.swap_node(&n8, &n7);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n4, &n5, -1);
	CHECK_LAST_NODE(n3, &n4);
	CHECK_NODE(n4, &n3, NULL, &n2, 1);
	CHECK_NODE(n5, &n2, &n8, &n7, 1);
	CHECK_LAST_NODE(n6, &n8);
	CHECK_NODE(n7, &n5, &n10, NULL, 1);
	CHECK_NODE(n8, &n6, NULL, &n5, 1);
	CHECK_LAST_NODE(n9, &n10);
	CHECK_NODE(n10, &n9, &n11, &n7, -1);
	CHECK_NODE(n11, NULL, &n12, &n10, -1);
	CHECK_LAST_NODE(n12, &n11);
	CPPUNIT_ASSERT(tree.root() == &n7);
}

void avl_tree_test_class::avl_root_swap_node_left4(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4), n5(5, 5), n6(6, 6), n7(7, 7), n8(8, 8), n9(9, 9), n10(10, 10), n11(11, 11), n12(12, 12);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n10);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n9);
	add_tree_node(tree, &n11);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n12);
	add_tree_node(tree, &n3);
	tree.swap_node(&n5, &n4);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n5, &n4, -1);
	CHECK_LAST_NODE(n3, &n5);
	CHECK_NODE(n4, &n2, &n7, &n8, 1);
	CHECK_NODE(n5, &n3, NULL, &n2, 1);
	CHECK_LAST_NODE(n6, &n7);
	CHECK_NODE(n7, &n6, NULL, &n4, 1);
	CHECK_NODE(n8, &n4, &n10, NULL, 1);
	CHECK_LAST_NODE(n9, &n10);
	CHECK_NODE(n10, &n9, &n11, &n8, -1);
	CHECK_NODE(n11, NULL, &n12, &n10, -1);
	CHECK_LAST_NODE(n12, &n11);
}

void avl_tree_test_class::avl_root_swap_node_left_son1(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	tree.swap_node(&n2, &n1);
	CHECK_LAST_NODE(n2, &n1);
	CHECK_NODE(n1, &n2, NULL, NULL, 1);
	CPPUNIT_ASSERT(tree.root() == &n1);
}

void avl_tree_test_class::avl_root_swap_node_left_son2(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n2, &n1);
	CHECK_LAST_NODE(n2, &n1);
	CHECK_LAST_NODE(n3, &n1);
	CHECK_NODE(n1, &n2, &n3, NULL, 0);
	CPPUNIT_ASSERT(tree.root() == &n1);
}

void avl_tree_test_class::avl_root_swap_node_left_son3(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);
	tree.swap_node(&n3, &n2);
	CHECK_LAST_NODE(n1, &n3);
	CHECK_NODE(n2, &n3, &n4, NULL, 1);
	CHECK_NODE(n3, &n1, NULL, &n2, 1);
	CHECK_LAST_NODE(n4, &n2);
	CPPUNIT_ASSERT(tree.root() == &n2);
}

void avl_tree_test_class::avl_root_swap_node_left_son4(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4), n5(5, 5);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n2, &n1);
	CHECK_NODE(n1, &n2, &n3, &n4, 0);
	CHECK_LAST_NODE(n2, &n1);
	CHECK_LAST_NODE(n3, &n1);
	CHECK_NODE(n4, &n1, &n5, NULL, 1);
	CHECK_LAST_NODE(n5, &n4);
}

void avl_tree_test_class::avl_root_swap_node_left_son5(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4), n5(5, 5), n6(6, 6), n7(7, 7);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n1);
	tree.swap_node(&n3, &n2);
	CHECK_LAST_NODE(n1, &n3);
	CHECK_NODE(n3, &n1, NULL, &n2, 1);
	CHECK_NODE(n2, &n3, &n4, &n5, 1);
	CHECK_LAST_NODE(n4, &n2);
	CHECK_NODE(n5, &n2, &n6, NULL, 1);
	CHECK_NODE(n6, NULL, &n7, &n5, -1);
	CHECK_LAST_NODE(n7, &n6);
}

void avl_tree_test_class::avl_root_swap_node_left_son6(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);
	tree.swap_node(&n2, &n1);
	
	CHECK_LAST_NODE(n2, &n1);
	CHECK_NODE(n1, &n2, NULL, &n3, 1);
	CHECK_NODE(n3, &n1, &n4, NULL, 1);
	CHECK_LAST_NODE(n4, &n3);
}
#define CHECK_NODE_RV(n, r, l, f, nb) CHECK_NODE(n, l, r, f, -1*nb)
void avl_tree_test_class::avl_root_swap_node_right1(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4), n5(-5, 5), n6(-6, 6), n7(-7, 7), n8(-8, 8);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n6, &n5);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE_RV(n2, &n1, &n3, &n4, 0);
	CHECK_LAST_NODE(n3, &n2);
	CHECK_NODE_RV(n4, &n2, &n6, &n5, 1);
	CHECK_NODE_RV(n5, &n4, &n7, NULL, 1);
	CHECK_LAST_NODE(n6, &n4);
	CHECK_NODE_RV(n7, NULL, &n8, &n5, -1);
	CHECK_LAST_NODE(n8, &n7);
	CPPUNIT_ASSERT(tree.root() == &n5);
}

void avl_tree_test_class::avl_root_swap_node_right2(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4), n5(-5, 5), n6(-6, 6), n7(-7, 7), n8(-8, 8);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n4, &n3);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE_RV(n2, &n1, &n4, &n3, 0);
	CHECK_NODE_RV(n3, &n2, &n5, &n6, 1);
	CHECK_LAST_NODE(n4, &n2);
	CHECK_LAST_NODE(n5, &n3);
	CHECK_NODE_RV(n6, &n3, &n7, NULL, 1);
	CHECK_NODE_RV(n7, NULL, &n8, &n6, -1);
	CHECK_LAST_NODE(n8, &n7);
}

void avl_tree_test_class::avl_root_swap_node_right3(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4), n5(-5, 5), n6(-6, 6), n7(-7, 7), n8(-8, 8), n9(-9, 9), n10(-10, 10), n11(-11, 11), n12(-12, 12);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n10);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n9);
	add_tree_node(tree, &n11);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n12);
	add_tree_node(tree, &n3);
	tree.swap_node(&n8, &n7);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE_RV(n2, &n1, &n4, &n5, -1);
	CHECK_LAST_NODE(n3, &n4);
	CHECK_NODE_RV(n4, &n3, NULL, &n2, 1);
	CHECK_NODE_RV(n5, &n2, &n8, &n7, 1);
	CHECK_LAST_NODE(n6, &n8);
	CHECK_NODE_RV(n7, &n5, &n10, NULL, 1);
	CHECK_NODE_RV(n8, &n6, NULL, &n5, 1);
	CHECK_LAST_NODE(n9, &n10);
	CHECK_NODE_RV(n10, &n9, &n11, &n7, -1);
	CHECK_NODE_RV(n11, NULL, &n12, &n10, -1);
	CHECK_LAST_NODE(n12, &n11);
	CPPUNIT_ASSERT(tree.root() == &n7);
}

void avl_tree_test_class::avl_root_swap_node_right4(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4), n5(-5, 5), n6(-6, 6), n7(-7, 7), n8(-8, 8), n9(-9, 9), n10(-10, 10), n11(-11, 11), n12(-12, 12);
	add_tree_node(tree, &n8);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n10);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n9);
	add_tree_node(tree, &n11);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n12);
	add_tree_node(tree, &n3);
	tree.swap_node(&n5, &n4);
	
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE_RV(n2, &n1, &n5, &n4, -1);
	CHECK_LAST_NODE(n3, &n5);
	CHECK_NODE_RV(n4, &n2, &n7, &n8, 1);
	CHECK_NODE_RV(n5, &n3, NULL, &n2, 1);
	CHECK_LAST_NODE(n6, &n7);
	CHECK_NODE_RV(n7, &n6, NULL, &n4, 1);
	CHECK_NODE_RV(n8, &n4, &n10, NULL, 1);
	CHECK_LAST_NODE(n9, &n10);
	CHECK_NODE_RV(n10, &n9, &n11, &n8, -1);
	CHECK_NODE_RV(n11, NULL, &n12, &n10, -1);
	CHECK_LAST_NODE(n12, &n11);
}

void avl_tree_test_class::avl_root_swap_node_right_son1(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	tree.swap_node(&n2, &n1);
	CHECK_LAST_NODE(n2, &n1);
	CHECK_NODE_RV(n1, &n2, NULL, NULL, 1);
	CPPUNIT_ASSERT(tree.root() == &n1);
}

void avl_tree_test_class::avl_root_swap_node_right_son2(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n2, &n1);
	CHECK_LAST_NODE(n2, &n1);
	CHECK_LAST_NODE(n3, &n1);
	CHECK_NODE_RV(n1, &n2, &n3, NULL, 0);
	CPPUNIT_ASSERT(tree.root() == &n1);
}

void avl_tree_test_class::avl_root_swap_node_right_son3(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);
	tree.swap_node(&n3, &n2);
	CHECK_LAST_NODE(n1, &n3);
	CHECK_NODE_RV(n2, &n3, &n4, NULL, 1);
	CHECK_NODE_RV(n3, &n1, NULL, &n2, 1);
	CHECK_LAST_NODE(n4, &n2);
	CPPUNIT_ASSERT(tree.root() == &n2);
}

void avl_tree_test_class::avl_root_swap_node_right_son4(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4), n5(-5, 5);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	tree.swap_node(&n2, &n1);
	CHECK_NODE_RV(n1, &n2, &n3, &n4, 0);
	CHECK_LAST_NODE(n2, &n1);
	CHECK_LAST_NODE(n3, &n1);
	CHECK_NODE_RV(n4, &n1, &n5, NULL, 1);
	CHECK_LAST_NODE(n5, &n4);
}

void avl_tree_test_class::avl_root_swap_node_right_son5(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4), n5(-5, 5), n6(-6, 6), n7(-7, 7);
	add_tree_node(tree, &n5);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n6);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n7);
	add_tree_node(tree, &n1);
	tree.swap_node(&n3, &n2);
	CHECK_LAST_NODE(n1, &n3);
	CHECK_NODE_RV(n3, &n1, NULL, &n2, 1);
	CHECK_NODE_RV(n2, &n3, &n4, &n5, 1);
	CHECK_LAST_NODE(n4, &n2);
	CHECK_NODE_RV(n5, &n2, &n6, NULL, 1);
	CHECK_NODE_RV(n6, NULL, &n7, &n5, -1);
	CHECK_LAST_NODE(n7, &n6);
}

void avl_tree_test_class::avl_root_swap_node_right_son6(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);
	tree.swap_node(&n2, &n1);
	
	CHECK_LAST_NODE(n2, &n1);
	CHECK_NODE_RV(n1, &n2, NULL, &n3, 1);
	CHECK_NODE_RV(n3, &n1, &n4, NULL, 1);
	CHECK_LAST_NODE(n4, &n3);
}

void avl_tree_test_class::avl_tree_remove_root_only(){
	int_tree tree;
	int_tree::node n(0,0);
	add_tree_node(tree, &n);
	CPPUNIT_ASSERT(&n == tree.remove(0));
	CHECK_LAST_NODE(n, NULL);
	CPPUNIT_ASSERT(!tree.first());
	CPPUNIT_ASSERT(!tree.last());
	CPPUNIT_ASSERT(!tree.root());
	CPPUNIT_ASSERT(!tree.size());
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_left_last_node(){
	int_tree tree;
	int_tree::node n1(0,0), n2(1,1);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n2);
	CPPUNIT_ASSERT(&n2 == tree.remove(1));
	CHECK_LAST_NODE(n2, NULL);
	CHECK_LAST_NODE(n1, NULL);
	CPPUNIT_ASSERT(tree.last() == &n1);
	CPPUNIT_ASSERT(tree.root() == &n1);
	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.size() == 1);
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_right_last_node(){
	int_tree tree;
	int_tree::node n1(0,0), n2(1,1);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	CPPUNIT_ASSERT(&n1 == tree.remove(0));
	CHECK_LAST_NODE(n2, NULL);
	CHECK_LAST_NODE(n1, NULL);
	CPPUNIT_ASSERT(tree.last() == &n2);
	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.first() == &n2);
	CPPUNIT_ASSERT(tree.size() == 1);
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_left_last_node_bl(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n4);

	CPPUNIT_ASSERT(&n1 == tree.remove(1));
	
	CHECK_LAST_NODE(n1, NULL);
	CHECK_LAST_NODE(n2, &n3);
	CHECK_NODE(n3, &n2, &n4, NULL, 0);
	CHECK_LAST_NODE(n4, &n3);

	CPPUNIT_ASSERT(tree.first() == &n2);
	CPPUNIT_ASSERT(tree.root() == &n3);
	CPPUNIT_ASSERT(tree.last() == &n4);
	CPPUNIT_ASSERT(tree.size() == 3);
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_right_last_node_bl(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);

	CPPUNIT_ASSERT(&n4 == tree.remove(4));
	
	CHECK_LAST_NODE(n4, NULL);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n3, NULL, 0);
	CHECK_LAST_NODE(n3, &n2);

	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.last() == &n3);
	CPPUNIT_ASSERT(tree.size() == 3);
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_left_swap(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3), n4(4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);
	
	CPPUNIT_ASSERT(&n3 == tree.remove(3));
	
	CHECK_LAST_NODE(n3, NULL);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE(n2, &n1, &n4, NULL, 0);
	CHECK_LAST_NODE(n4, &n2);

	CPPUNIT_ASSERT(tree.first() == &n1);
	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.last() == &n4);
	CPPUNIT_ASSERT(tree.size() == 3);
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_right_swap(){
	int_tree tree;
	int_tree::node n1(-1, 1), n2(-2, 2), n3(-3, 3), n4(-4, 4);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n4);
	add_tree_node(tree, &n1);
	
	CPPUNIT_ASSERT(&n3 == tree.remove(-3));
	
	CHECK_LAST_NODE(n3, NULL);
	CHECK_LAST_NODE(n1, &n2);
	CHECK_NODE_RV(n2, &n1, &n4, NULL, 0);
	CHECK_LAST_NODE(n4, &n2);

	CPPUNIT_ASSERT(tree.first() == &n4);
	CPPUNIT_ASSERT(tree.root() == &n2);
	CPPUNIT_ASSERT(tree.last() == &n1);
	CPPUNIT_ASSERT(tree.size() == 3);
	validation_tree(tree);
}

void avl_tree_test_class::avl_tree_remove_root_middle(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2), n3(3, 3);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n3);
	add_tree_node(tree, &n1);
	CPPUNIT_ASSERT(&n2 == tree.remove(2));
	CHECK_LAST_NODE(n2, NULL);
	CHECK_LAST_NODE(n3, &n1);
	CHECK_NODE(n1, NULL, &n3, NULL, -1);
}

void avl_tree_test_class::avl_tree_remove_root_left(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2);
	add_tree_node(tree, &n2);
	add_tree_node(tree, &n1);
	CPPUNIT_ASSERT(&n2 == tree.remove(2));
	CHECK_LAST_NODE(n2, NULL);
	CHECK_LAST_NODE(n1, NULL);
}

void avl_tree_test_class::avl_tree_remove_root_right(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n2);
	CPPUNIT_ASSERT(&n1 == tree.remove(1));
	CHECK_LAST_NODE(n2, NULL);
	CHECK_LAST_NODE(n1, NULL);
}

void avl_tree_test_class::avl_tree_remove_nodes(){
	int_tree tree;
	int_tree::node** ns = new int_tree::node*[TREE_ADD_COUNT];
	int *index = new int[TREE_ADD_COUNT];
	for(int i=0; i<TREE_ADD_COUNT; i++){
		ns[i] = new int_tree::node(i, i);
		index[i] = i;
	}

	for(int i=0;i<TREE_ADD_COUNT; i++){
		int i1 = rand() & (TREE_ADD_COUNT - 1);
		int i2 = rand() & (TREE_ADD_COUNT - 1);
		std::swap(index[i1], index[i2]);
	}

	for(int i=0; i<TREE_ADD_COUNT;i++){
		add_tree_node(tree, ns[i]);
	}

	for(int i=0; i<TREE_ADD_COUNT; i++){
		int_tree::node* n = tree.remove(index[i]);
		CPPUNIT_ASSERT(n);
		CPPUNIT_ASSERT(n->value() == index[i]);
		CPPUNIT_ASSERT(tree.size() == (size_t)TREE_ADD_COUNT-1-i);
		validation_tree(tree);
	}
	tree.clear();
	for(int i=0; i<TREE_ADD_COUNT; i++){
		delete ns[i];
	}
	delete[] ns;
	delete[] index;
}

void avl_tree_test_class::avl_tree_clear(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n2);
	tree.clear();
	CPPUNIT_ASSERT(!tree.first());
	CPPUNIT_ASSERT(!tree.last());
	CPPUNIT_ASSERT(!tree.root());
	CPPUNIT_ASSERT(!tree.size());

	CHECK_LAST_NODE(n1, NULL);
	CHECK_LAST_NODE(n2, NULL);
}

void avl_tree_test_class::avl_tree_discard(){
	int_tree tree;
	int_tree::node n1(1, 1), n2(2, 2);
	add_tree_node(tree, &n1);
	add_tree_node(tree, &n2);
	tree.discard();
	CPPUNIT_ASSERT(!tree.first());
	CPPUNIT_ASSERT(!tree.last());
	CPPUNIT_ASSERT(!tree.root());
	CPPUNIT_ASSERT(!tree.size());

	CHECK_LAST_NODE(n1, NULL);
	CHECK_LAST_NODE(n2, NULL);

	CPPUNIT_ASSERT(n1.next() == &n2);
	CPPUNIT_ASSERT(n2.previous() == &n1);
}

