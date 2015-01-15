#include "cclibc_test/util/array_list_test.h"
#include "cclibc/util/array_list.h"

using CCLIBC::array_list;

void array_list_test_class::array_list_empty(){
	array_list<int> l;
	CPPUNIT_ASSERT(!l.size());
	CPPUNIT_ASSERT(!l.capcity());
}

void array_list_test_class::array_list_append_first(){
	array_list<int, test_util_allocator> l;
	test_util_allocator::reset();
	l.add(100);
	CPPUNIT_ASSERT(l.size() == 1);
	CPPUNIT_ASSERT(l.capcity() == 1);
	CPPUNIT_ASSERT(l[0] == 100);
	CPPUNIT_ASSERT(test_util_allocator::allocated() == sizeof(int)*l.capcity());
}

void array_list_test_class::array_list_append_realloc(){
	array_list<int, test_util_allocator> l;
	test_util_allocator::reset();
	l.add(100);
	l.add(200);
	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.capcity() == 2);
	CPPUNIT_ASSERT(l[0] == 100);
	CPPUNIT_ASSERT(l[1] == 200);
	CPPUNIT_ASSERT(test_util_allocator::allocated() == sizeof(int)*l.capcity());
}

void array_list_test_class::array_list_append_realloc_cap(){
	array_list<int, test_util_allocator> l;
	test_util_allocator::reset();
	for(int i=0; i<5; i++){
		l.add(i);
	}
	CPPUNIT_ASSERT(l.size() == 5);
	CPPUNIT_ASSERT(l.capcity() == 8);
	for(int i=0; i<5; i++){
		CPPUNIT_ASSERT(l[i] == i);
	}
	CPPUNIT_ASSERT(test_util_allocator::allocated() == sizeof(int)*l.capcity());
}

void array_list_test_class::array_list_clear(){
	array_list<int, test_util_allocator> l;
	test_util_allocator::reset();
	l.add(0);
	l.clear();
	CPPUNIT_ASSERT(!test_util_allocator::allocated());
}

namespace{
	class test_class{
	public:
		static bool start_test;
		static size_t pos;
		static test_class* buf[3];
		~test_class(){
			if(start_test){
				buf[pos++] = this;
			}
		}
	};
	bool test_class::start_test;
	size_t test_class::pos;
	test_class* test_class::buf[3];
}

void array_list_test_class::array_list_clear_deconstruc(){
	array_list<test_class> l;
	l.add(test_class());
	l.add(test_class());
	l.add(test_class());
	test_class* list[3];
	list[0] = &l[0];
	list[1] = &l[1];
	list[2] = &l[2];
	test_class::start_test = true;
	test_class::pos = 0;
	l.clear();
	CPPUNIT_ASSERT(list[0] == test_class::buf[2]);
	CPPUNIT_ASSERT(list[1] == test_class::buf[1]);
	CPPUNIT_ASSERT(list[2] == test_class::buf[0]);
}

void array_list_test_class::array_list_deconstruct(){
	test_class* list[3];
	{
		array_list<test_class> l;
		l.add(test_class());
		l.add(test_class());
		l.add(test_class());
		list[0] = &l[0];
		list[1] = &l[1];
		list[2] = &l[2];
		test_class::start_test = true;
		test_class::pos = 0;
	}
	CPPUNIT_ASSERT(list[0] == test_class::buf[2]);
	CPPUNIT_ASSERT(list[1] == test_class::buf[1]);
	CPPUNIT_ASSERT(list[2] == test_class::buf[0]);

}

