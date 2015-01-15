#include "cclibc_test/util/map_test.h"
#include "cclibc/util/map.h"

using CCLIBC::map;
using CCLIBC::illegal_argument;

void map_test_class::map_add(){
	map<int, int> m;
	static int TREE_ADD_COUNT=0x1000;
	for(int i=0;i<TREE_ADD_COUNT;i++){
		m[i] = i + 1;
	}
	for(int i=0;i<TREE_ADD_COUNT;i++){
		CPPUNIT_ASSERT(m[i] == i + 1);
	}
	CPPUNIT_ASSERT((int)m.size() == TREE_ADD_COUNT);
}

void map_test_class::map_add_fun(){
	map<int, int> m;
	static int TREE_ADD_COUNT=0x1000;
	for(int i=0;i<TREE_ADD_COUNT;i++){
		m.add(i, i + 1);
	}
	for(int i=0;i<TREE_ADD_COUNT;i++){
		CPPUNIT_ASSERT(m.get(i) == i + 1);
	}
	CPPUNIT_ASSERT((int)m.size() == TREE_ADD_COUNT);

}

namespace{
static bool inited = false;
static bool desed = false;
class  value_obj{
public:
	value_obj(){
		inited = true;
	}

	~value_obj(){
		desed = true;
	}
};
}

void map_test_class::map_default(){
	inited = false;
	desed = false;
	map<int, value_obj> m;
	m[0];
	CPPUNIT_ASSERT(inited);
	CPPUNIT_ASSERT(desed);	
}

void map_test_class::map_contains(){
	map<int, int> m;
	CPPUNIT_ASSERT(!m.contains(1));
	m[1];
	CPPUNIT_ASSERT(m.contains(1));
}

typedef map<int, int> map_int_int;
void map_test_class::map_clear(){
	map<int, int>::allocator_type::instance().clear();
	map<int, int> m;
	m.add(0,0);
	m.add(1,1);
	m.clear();
	CPPUNIT_ASSERT(!m.size());
	CPPUNIT_ASSERT(map_int_int::allocator_type::instance()._active_pages.first()->gathered());
}

void map_test_class::map_des(){
	map<int, int>::allocator_type::instance().clear();
	{
		map<int, int> m;
		m.add(0,0);
		m.add(1,1);
	}
	CPPUNIT_ASSERT(map_int_int::allocator_type::instance()._active_pages.first()->gathered());
}
void map_test_class::map_foreach(){
	map<int, int> m;
	m.add(0, 1);
	m.add(1, 2);
	int j = 0;
	LIST_FOREACH(map_int_int::entry, e, m){
		CPPUNIT_ASSERT(e->key() == j);
		CPPUNIT_ASSERT(e->value() == j+1);
		++j;
	}
}

void map_test_class::map_c_foreach(){
	map<int, int> m;
	m.add(0, 1);
	m.add(1, 2);
	const map<int, int> &cm = m;
	int j = 0;
	LIST_FOREACH(const map_int_int::entry, e, cm){
		CPPUNIT_ASSERT(e->key() == j);
		CPPUNIT_ASSERT(e->value() == j+1);
		++j;
	}
}

void map_test_class::map_rforeach(){
	map<int, int> m;
	m.add(0, 1);
	m.add(1, 2);
	int j = 1;
	LIST_FOREACH_R(map_int_int::entry, e, m){
		CPPUNIT_ASSERT(e->key() == j);
		CPPUNIT_ASSERT(e->value() == j+1);
		--j;
	}
}

void map_test_class::map_c_rforeach(){
	map<int, int> m;
	m.add(0, 1);
	m.add(1, 2);
	const map<int, int> &cm = m;
	int j = 1;
	LIST_FOREACH_R(const map_int_int::entry, e, cm){
		CPPUNIT_ASSERT(e->key() == j);
		CPPUNIT_ASSERT(e->value() == j+1);
		--j;
	}
}

void map_test_class::map_remove(){
	map_int_int::allocator_type::instance().clear();
	map_int_int m;
	m.add(0,0);
	m.add(1,1);
	CPPUNIT_ASSERT(0 == m.remove(0));
	CPPUNIT_ASSERT(1 == map_int_int::allocator_type::instance()._active_pages.first()->_allocated);
	CPPUNIT_ASSERT(m.contains(1));
	CPPUNIT_ASSERT(!m.contains(0));
}

void map_test_class::map_remove_exp(){
	map_int_int m;
	CPPUNIT_ASSERT_THROW(m.remove(0), illegal_argument*);
}

void map_test_class::map_erase(){
	map_int_int::allocator_type::instance().clear();
	map_int_int m;
	m.add(0,0);
	m.add(1,1);
	m.erase(0);
	CPPUNIT_ASSERT(1 == map_int_int::allocator_type::instance()._active_pages.first()->_allocated);
	CPPUNIT_ASSERT(m.contains(1));
	CPPUNIT_ASSERT(!m.contains(0));
}

