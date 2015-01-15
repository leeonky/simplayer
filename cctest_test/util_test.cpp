#include "cctest_test/util_test.h"

namespace{
void test_fun(int i, int j, int, int, int k){
}
int rtest_fun(int i, int j, int, int, int k){
	return k;
}
void test_fun(int i, int j, int, int){
}
int rtest_fun(int i, int j, int, int k){
	return k;
}
void test_fun(int i, int j, int){
}
int rtest_fun(int i, int j, int k){
	return k;
}
void test_fun(int i, int){
}
int rtest_fun(int i, int k){
	return k;
}
void test_fun(int i){
}
int rtest_fun(int k){
	return k;
}
void test_fun(){
}
int rtest_fun(){
	return -1;
}
int rtest_fun(int i, int j, int, int, int, int k){
	return -1;
}
INTERFACE_TESTABLE_FUNCTION_6(int, trtest_fun6, rtest_fun, int, int, int, int, int, int)

INTERFACE_TESTABLE_VOID_FUNCTION_5(ttest_fun5, test_fun, int, int, int, int, int)
INTERFACE_TESTABLE_FUNCTION_5(int, trtest_fun5, rtest_fun, int, int, int, int, int)

INTERFACE_TESTABLE_VOID_FUNCTION_4(ttest_fun4, test_fun, int, int, int, int)
INTERFACE_TESTABLE_FUNCTION_4(int, trtest_fun4, rtest_fun, int, int, int, int)

INTERFACE_TESTABLE_VOID_FUNCTION_3(ttest_fun3, test_fun, int, int, int)
INTERFACE_TESTABLE_FUNCTION_3(int, trtest_fun3, rtest_fun, int, int, int)

INTERFACE_TESTABLE_VOID_FUNCTION_2(ttest_fun2, test_fun, int, int)
INTERFACE_TESTABLE_FUNCTION_2(int, trtest_fun2, rtest_fun, int, int)

INTERFACE_TESTABLE_VOID_FUNCTION_1(ttest_fun1, test_fun, int)
INTERFACE_TESTABLE_FUNCTION_1(int, trtest_fun1, rtest_fun, int)

INTERFACE_TESTABLE_VOID_FUNCTION(ttest_fun0, test_fun)
INTERFACE_TESTABLE_FUNCTION(int, trtest_fun0, rtest_fun)

}
void util_test_class::interface_testor_test(){
	interface_testor::clear();
	ttest_fun5(1,2,3,4,5);
	ttest_fun4(1,2,3,4);
	ttest_fun3(1,2,3);
	ttest_fun2(1,2);
	ttest_fun1(1);
	ttest_fun0();
	
	trtest_fun5(1,2,3,4,5);
	trtest_fun4(1,2,3,4);
	trtest_fun3(1,2,3);
	trtest_fun2(1,2);
	trtest_fun1(1);
	trtest_fun0();
	
	trtest_fun6(1,2,3,4,5,6);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("ttest_fun5"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(3));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(4));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(5));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("ttest_fun4"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(3));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(3).equal(4));
	
	CPPUNIT_ASSERT(interface_testor::method(2).equal("ttest_fun3"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(2).equal(3));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("ttest_fun2"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(2));
	
	CPPUNIT_ASSERT(interface_testor::method(4).equal("ttest_fun1"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(1));
	
	CPPUNIT_ASSERT(interface_testor::method(5).equal("ttest_fun0"));
	
	CPPUNIT_ASSERT(interface_testor::method(6).equal("trtest_fun5"));
	CPPUNIT_ASSERT(interface_testor::method(6).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(6).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(6).parameter(2).equal(3));
	CPPUNIT_ASSERT(interface_testor::method(6).parameter(3).equal(4));
	CPPUNIT_ASSERT(interface_testor::method(6).parameter(4).equal(5));
	CPPUNIT_ASSERT(interface_testor::method(6).get_return().equal(5));
	
	CPPUNIT_ASSERT(interface_testor::method(7).equal("trtest_fun4"));
	CPPUNIT_ASSERT(interface_testor::method(7).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(7).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(7).parameter(2).equal(3));
	CPPUNIT_ASSERT(interface_testor::method(7).parameter(3).equal(4));
	CPPUNIT_ASSERT(interface_testor::method(7).get_return().equal(4));
	
	CPPUNIT_ASSERT(interface_testor::method(8).equal("trtest_fun3"));
	CPPUNIT_ASSERT(interface_testor::method(8).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(8).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(8).parameter(2).equal(3));
	CPPUNIT_ASSERT(interface_testor::method(8).get_return().equal(3));
	
	CPPUNIT_ASSERT(interface_testor::method(9).equal("trtest_fun2"));
	CPPUNIT_ASSERT(interface_testor::method(9).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(9).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(9).get_return().equal(2));
	
	CPPUNIT_ASSERT(interface_testor::method(10).equal("trtest_fun1"));
	CPPUNIT_ASSERT(interface_testor::method(10).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(10).get_return().equal(1));
	
	CPPUNIT_ASSERT(interface_testor::method(11).equal("trtest_fun0"));
	CPPUNIT_ASSERT(interface_testor::method(11).get_return().equal(-1));
	
	CPPUNIT_ASSERT(interface_testor::method(12).equal("trtest_fun6"));
	CPPUNIT_ASSERT(interface_testor::method(12).parameter(0).equal(1));
	CPPUNIT_ASSERT(interface_testor::method(12).parameter(1).equal(2));
	CPPUNIT_ASSERT(interface_testor::method(12).parameter(2).equal(3));
	CPPUNIT_ASSERT(interface_testor::method(12).parameter(3).equal(4));
	CPPUNIT_ASSERT(interface_testor::method(12).parameter(4).equal(5));
	CPPUNIT_ASSERT(interface_testor::method(12).parameter(5).equal(6));
	CPPUNIT_ASSERT(interface_testor::method(12).get_return().equal(-1));
}

