#include "cclibc_test/memory_test.h"
#include "cclibc/memory.h"
#include "cclibc/thread/thread.h"

using CCLIBC::stack_page;
using CCLIBC::single_list;
using CCLIBC::object_pool;
using CCLIBC::stack_pool;
using CCLIBC::stack_pool_base;
using CCLIBC::buffer_pool;
using CCLIBC::buffer_pool_base;
using CCLIBC::allocator;
using CCLIBC::memory;
using CCLIBC::util;
using CCLIBC::thread;
using CCLIBC::stack_ptr;
using CCLIBC::collect_node;
using CCLIBC::used_node;
using CCLIBC::object_ptr;
using CCLIBC::buffer_page;
using CCLIBC::dual_list;
using CCLIBC::buffer_pool_list;
using CCLIBC::pool_ptr;

void memory_test_class::memory_malloc(){
	void *p = memory::malloc_nothrow(0);
	CPPUNIT_ASSERT(p);	
	memory::free(p);
}

void memory_test_class::stack_page_allocate(){
	uint8_t buf[128];
	stack_page p(buf,sizeof(buf));
	CPPUNIT_ASSERT(p.left() == sizeof(buf));
	CPPUNIT_ASSERT(p.size() == sizeof(buf));
	CPPUNIT_ASSERT(p.allocate_nothrow(8) == buf);
	CPPUNIT_ASSERT(p.left() == sizeof(buf) - 8);
	
	CPPUNIT_ASSERT(p.enough(sizeof(buf) - 8));
	CPPUNIT_ASSERT(p.allocate_nothrow(sizeof(buf) - 8 - sizeof(void*)));
	CPPUNIT_ASSERT(!p.full());
	p.allocate_nothrow(1);
	CPPUNIT_ASSERT(p.full());
}

class test_stack_pool: public stack_pool_base{
public:
	const single_list<stack_page>& active_pages() const{
		return _active_pages;
	}
	const single_list<stack_page>& full_pages() const{
		return _full_pages;
	}
	size_t page_count() const {
		return _page_count;
	}
	size_t base_page_size() const {
		return _base_page_size;
	}
};

void memory_test_class::stack_pool_base_properties() {
	test_stack_pool spl;
	CPPUNIT_ASSERT(spl.page_count() == 1);
	CPPUNIT_ASSERT(spl.base_page_size() == POOL_PAGE_SIZE);

	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(!spl.full_pages().size());
}

void memory_test_class::stack_pool_base_allocate_new_page() {
	test_stack_pool sp1;
	uint8_t *p = static_cast<uint8_t*>(sp1.allocate_nothrow(1));

	CPPUNIT_ASSERT(sp1.active_pages().first() == static_cast<void*>(p - ALIGNED_SIZEOF(stack_page)));
	CPPUNIT_ASSERT(1 == sp1.active_pages().size());
	CPPUNIT_ASSERT(POOL_PAGE_SIZE == sp1.active_pages().first()->size() + ALIGNED_SIZEOF(stack_page));
}

void memory_test_class::stack_pool_base_allocate_reuse_page(){
	test_stack_pool sp1;
	sp1.allocate_nothrow(1);
	sp1.allocate_nothrow(1);

	CPPUNIT_ASSERT(1 == sp1.active_pages().size());
	CPPUNIT_ASSERT(sp1.active_pages().first()->left() == POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page) - sizeof(void*)*2);
}

void memory_test_class::stack_pool_base_allocate_new_page2(){
	test_stack_pool sp1;
	sp1.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page) - 48);
	sp1.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page) - 48);

	CPPUNIT_ASSERT(2 == sp1.active_pages().size());
	LIST_FOREACH(const stack_page, p, sp1.active_pages()){
		CPPUNIT_ASSERT(p->left() == 48);
	}
}

void memory_test_class::stack_pool_base_allocate_new_page_fullalloc(){
	test_stack_pool spl;
	spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(1 == spl.full_pages().size());
}

void memory_test_class::stack_pool_base_allocate_new_page_fullalloc2(){
	test_stack_pool spl;
	spl.allocate_nothrow(1);
	spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page) - sizeof(void(*)));
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(1 == spl.full_pages().size());
}

void memory_test_class::stack_pool_base_allocate_str(){
	test_stack_pool spl;
	const char *str = spl.allocate_nothrow("Hello %d", 123);
	CPPUNIT_ASSERT(!strcmp(str, "Hello 123"));
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == (POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page) - util::algin<size_t, sizeof(void*)>(10)));
}

void memory_test_class::stack_pool_base_allocate_str_len(){
	test_stack_pool spl;
	size_t len;
	const char *str = spl.allocate(len, "Hello %d", 123);
	CPPUNIT_ASSERT(!strcmp(str, "Hello 123"));
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == (POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page) - util::algin<size_t, sizeof(void*)>(10)));
	CPPUNIT_ASSERT(9 == len);
}

void memory_test_class::stack_pool_clear_n_m_active(){
	test_stack_pool spl;
	spl.allocate(10);
	spl.clear();
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(!spl.full_pages().size());
}

void memory_test_class::stack_pool_clear_n_m_k_active(){
	test_stack_pool spl;
	spl.allocate(POOL_PAGE_SIZE/2);
	spl.allocate(POOL_PAGE_SIZE/2);
	spl.clear();
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(!spl.full_pages().size());
}

void memory_test_class::stack_pool_clear_n_full(){
	test_stack_pool spl;
	spl.allocate(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.clear();
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(!spl.full_pages().size());
}

void memory_test_class::stack_pool_clear_n_fulls(){
	test_stack_pool spl;
	spl.allocate(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.allocate(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.clear();
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(!spl.full_pages().size());
}

void memory_test_class::stack_pool_base_reset0(){
	test_stack_pool spl;
	spl.reset();
	CPPUNIT_ASSERT(!spl.active_pages().size());
	CPPUNIT_ASSERT(!spl.full_pages().size());
}

void memory_test_class::stack_pool_base_reset1() {
	test_stack_pool spl;
	void * p1 = spl.allocate_nothrow(1);
	CPPUNIT_ASSERT(spl.active_pages().size() == 1);
	spl.reset();
	CPPUNIT_ASSERT(spl.active_pages().size() == 1);
	CPPUNIT_ASSERT(!spl.full_pages().size());
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == spl.active_pages().first()->size());
	CPPUNIT_ASSERT(spl.allocate_nothrow(1) == p1);
}

void memory_test_class::stack_pool_base_reset2() {
	test_stack_pool spl;
	void * p1 = spl.allocate_nothrow(1);
	spl.allocate_nothrow(POOL_PAGE_SIZE);
	spl.allocate_nothrow(POOL_PAGE_SIZE);
	CPPUNIT_ASSERT(spl.active_pages().size() == 3);
	spl.reset();
	CPPUNIT_ASSERT(spl.active_pages().size() == 1);
	CPPUNIT_ASSERT(!spl.full_pages().size());
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == spl.active_pages().first()->size());
	CPPUNIT_ASSERT(spl.allocate_nothrow(1) == p1);
}

void memory_test_class::stack_pool_base_reset3() {
	test_stack_pool spl;
	void * p1 = spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	CPPUNIT_ASSERT(spl.full_pages().size() == 1);
	spl.reset();
	CPPUNIT_ASSERT(spl.active_pages().size() == 1);
	CPPUNIT_ASSERT(!spl.full_pages().size());
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == spl.active_pages().first()->size());
	CPPUNIT_ASSERT(spl.allocate_nothrow(1) == p1);
}

void memory_test_class::stack_pool_base_reset4() {
	test_stack_pool spl;
	void * p1 = spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	CPPUNIT_ASSERT(spl.full_pages().size() == 3);
	spl.reset();
	CPPUNIT_ASSERT(spl.active_pages().size() == 1);
	CPPUNIT_ASSERT(!spl.full_pages().size());
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == spl.active_pages().first()->size());
	CPPUNIT_ASSERT(spl.allocate_nothrow(1) == p1);
}

void memory_test_class::stack_pool_base_reset5() {
	test_stack_pool spl;
	void * p1 = spl.allocate_nothrow(1);
	spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.allocate_nothrow(POOL_PAGE_SIZE - ALIGNED_SIZEOF(stack_page));
	spl.allocate_nothrow(POOL_PAGE_SIZE);
	CPPUNIT_ASSERT(spl.full_pages().size() == 2);
	CPPUNIT_ASSERT(spl.active_pages().size() == 2);
	spl.reset();
	CPPUNIT_ASSERT(spl.active_pages().size() == 1);
	CPPUNIT_ASSERT(!spl.full_pages().size());
	CPPUNIT_ASSERT(spl.active_pages().first()->left() == spl.active_pages().first()->size());
	CPPUNIT_ASSERT(spl.allocate_nothrow(1) == p1);
}
namespace{
stack_pool *thread_pool;

void thread_pool_run(){
	thread_pool = &stack_pool::thread_pool();
}

}

void memory_test_class::stack_pool_thread_stack_pool(){
	thread t(TRACE_PARAMS(thread_pool_run));
	t.join();
	CPPUNIT_ASSERT(&stack_pool::thread_pool() != thread_pool);
}

namespace{
bool des = false;
class test_obj{
public:
	~test_obj(){
		des = true;
	}
};
}

void memory_test_class::stack_ptr_des(){
	stack_pool spl;
	des = false;
	{
		stack_ptr<test_obj> p(new (spl.allocate(sizeof(test_obj)))test_obj());
	}
	CPPUNIT_ASSERT(des);
}

void memory_test_class::stack_ptr_des_release(){
	stack_pool spl;
	des = false;
	test_obj *r = NULL;
	{
		stack_ptr<test_obj> p(new (spl.allocate(sizeof(test_obj)))test_obj());
		p.release();
		r = p.operator->();
	}
	CPPUNIT_ASSERT(!des);
	r->~test_obj();
}

void memory_test_class::stack_ptr_reassign(){
	stack_pool spl;
	des = false;
	stack_ptr<test_obj> p(new (spl.allocate(sizeof(test_obj)))test_obj());
	p.reassign(NULL);
	CPPUNIT_ASSERT(des);
}

void memory_test_class::buffer_pool_buffer_size(){
	size_t bl = 1;
	size_t es = util::algin<size_t, sizeof(void*)>(bl);
	buffer_pool_base p1(bl);
	CPPUNIT_ASSERT(p1.buffer_size() == std::max(es + ALIGNED_SIZEOF(used_node), ALIGNED_SIZEOF(collect_node)));

	bl = ALIGNED_SIZEOF(buffer_page) + 1;
	es = util::algin<size_t, sizeof(void*)>(bl);
	buffer_pool_base p2(bl);
	CPPUNIT_ASSERT(p2.buffer_size() == std::max(es + ALIGNED_SIZEOF(used_node), ALIGNED_SIZEOF(collect_node)));
}

void memory_test_class::buffer_pool_properties(){
	size_t bl = 256;
	size_t es = util::algin<size_t, sizeof(void*)>(bl) + ALIGNED_SIZEOF(used_node);
	buffer_pool_base pl(bl, 80);
	CPPUNIT_ASSERT(pl.buffer_size() == es);
	size_t ps = pl.page_size();
	CPPUNIT_ASSERT((ps - ALIGNED_SIZEOF(buffer_page))/es*es *100/ps >= 80);
	CPPUNIT_ASSERT(pl.buffer_count() == (ps - ALIGNED_SIZEOF(buffer_page))/es);
}

void memory_test_class::buffer_page_allocate(){
	uint8_t buf[1024];
	buffer_pool_base bp(128);
	buffer_page pl(buf, bp); 
	void *p = pl.retrieve();
	CPPUNIT_ASSERT(p == buf + ALIGNED_SIZEOF(used_node));
	CPPUNIT_ASSERT(pl.allocated() == 1);
}

void memory_test_class::buffer_page_full(){
	uint8_t buf[POOL_PAGE_SIZE];
	buffer_pool_base bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page) - ALIGNED_SIZEOF(used_node), 0);
	buffer_page pl(buf, bp);
	pl.retrieve();
	CPPUNIT_ASSERT(!pl.full());
	pl.retrieve();
	CPPUNIT_ASSERT(pl.full());
	CPPUNIT_ASSERT(pl.allocated() == 2);
}

void memory_test_class::buffer_page_deallocate(){
	uint8_t buf[1024];
	buffer_pool_base bp(128);
	buffer_page pl(buf, bp);
	void *p = pl.retrieve();
	CPPUNIT_ASSERT(pl.allocated() == 1);
	pl.deallocate(p);
	CPPUNIT_ASSERT(pl.released().size() == 1);
	CPPUNIT_ASSERT(!pl.allocated());
}

void memory_test_class::buffer_page_deallocate_full(){
	uint8_t buf[POOL_PAGE_SIZE];
	buffer_pool_base bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page) - ALIGNED_SIZEOF(used_node), 0);
	buffer_page pl(buf, bp);
	pl.retrieve();
	CPPUNIT_ASSERT(!pl.full());
	void * p = pl.retrieve();
	CPPUNIT_ASSERT(pl.full());
	CPPUNIT_ASSERT(pl.allocated() == 2);
	pl.deallocate(p);
	CPPUNIT_ASSERT(!pl.full());
	CPPUNIT_ASSERT(pl.allocated() == 1);
}

void memory_test_class::buffer_page_reallocate(){
	uint8_t buf[1024];
	buffer_pool_base bp(128);
	buffer_page pl(buf, bp);
	void *p = NULL;
	pl.deallocate(p = pl.retrieve());
	CPPUNIT_ASSERT(pl.retrieve() == p);
	CPPUNIT_ASSERT(!pl.released().size());
}

void memory_test_class::buffer_page_deallocate_all(){
	uint8_t buf[POOL_PAGE_SIZE];
	buffer_pool_base bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	buffer_page pl(buf, bp);
	void *p1 = pl.retrieve();
	void *p2 = pl.retrieve();
	pl.deallocate(p1);
	CPPUNIT_ASSERT(!pl.gathered());
	pl.deallocate(p2);
	CPPUNIT_ASSERT(pl.gathered());
}

class buffer_pool_t: public buffer_pool_base{
public:
	buffer_pool_t(size_t s, size_t u = 60):buffer_pool_base(s, u){}
	const dual_list<buffer_page>& active_pages() const{
		return _active_pages;
	}
	const dual_list<buffer_page>& full_pages() const{
		return _full_pages;
	}
};

void memory_test_class::buffer_pool_allocate_n_0_active_a_1_node(){
	buffer_pool_t pl(10);
	CPPUNIT_ASSERT(pl.active_pages().size() == 0);
	uint8_t *p = static_cast<uint8_t*>(pl.allocate());
	CPPUNIT_ASSERT(pl.active_pages().size() == 1);
	CPPUNIT_ASSERT(pl.active_pages().first() == static_cast<void*>(p - ALIGNED_SIZEOF(buffer_page) - ALIGNED_SIZEOF(used_node)));
}

void memory_test_class::buffer_pool_allocate_n_m_active_a_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE/4 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
	bp.allocate();
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
}

void memory_test_class::buffer_pool_allocate_n_n_1_active_a_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
	bp.allocate();
	CPPUNIT_ASSERT(!bp.active_pages().size());
	CPPUNIT_ASSERT(bp.full_pages().size() == 1);
}

void memory_test_class::buffer_pool_allocate_1_0_active_a_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE - ALIGNED_SIZEOF(buffer_page) - ALIGNED_SIZEOF(used_node), 0);
	bp.allocate();
	CPPUNIT_ASSERT(bp.full_pages().size() == 1);
}

void memory_test_class::buffer_pool_deallocate_4_1_active_de_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE/4 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	void *p = bp.allocate();
	bp.deallocate(p);
	CPPUNIT_ASSERT(bp.active_pages().first()->released().size() == 1);
}

void memory_test_class::buffer_pool_deallocate_2_1_active_de_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	void *p = bp.allocate();
	bp.deallocate(p);
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
}


void memory_test_class::buffer_pool_deallocate_2_1_1_active_de_2_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	void *p1 = bp.allocate();
	void *p2 = bp.allocate();
	void *p3 = bp.allocate();
	bp.deallocate(p2);
	CPPUNIT_ASSERT(bp.active_pages().size() == 2);
	bp.deallocate(p1);
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
	bp.deallocate(p3);
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
}

void memory_test_class::buffer_pool_deallocate_2_2_active_de_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	void *p = bp.allocate();
	bp.deallocate(p);
	CPPUNIT_ASSERT(bp.active_pages().first()->released().size() == 1);
}

void memory_test_class::buffer_pool_deallocate_1_1_active_de_1_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE - ALIGNED_SIZEOF(buffer_page), 0);
	void *p = bp.allocate();
	bp.deallocate(p);
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
	CPPUNIT_ASSERT(!bp.full_pages().size());
}

void memory_test_class::buffer_pool_deallocate_1_1_1_active_de_2_node(){
	buffer_pool_t bp(POOL_PAGE_SIZE - ALIGNED_SIZEOF(buffer_page) - ALIGNED_SIZEOF(used_node), 0);
	void *p1 = bp.allocate();
	void *p2 = bp.allocate();
	bp.deallocate(p1);
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
	CPPUNIT_ASSERT(bp.full_pages().size() == 1);
	bp.deallocate(p2);
	CPPUNIT_ASSERT(bp.active_pages().size() == 1);
	CPPUNIT_ASSERT(!bp.full_pages().size());
}

void memory_test_class::buffer_pool_clear_n_m_active(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	bp.clear();
	CPPUNIT_ASSERT(!bp.active_pages().size());
	CPPUNIT_ASSERT(!bp.full_pages().size());
}

void memory_test_class::buffer_pool_clear_n_m_k_active(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	void *p = bp.allocate();
	bp.allocate();
	bp.allocate();
	bp.deallocate(p);
	bp.clear();
	CPPUNIT_ASSERT(!bp.active_pages().size());
	CPPUNIT_ASSERT(!bp.full_pages().size());
}

void memory_test_class::buffer_pool_clear_n_full(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	bp.allocate();
	bp.clear();
	CPPUNIT_ASSERT(!bp.active_pages().size());
	CPPUNIT_ASSERT(!bp.full_pages().size());
}

void memory_test_class::buffer_pool_clear_n_fulls(){
	buffer_pool_t bp(POOL_PAGE_SIZE/2 - ALIGNED_SIZEOF(buffer_page), 0);
	bp.allocate();
	bp.allocate();
	bp.allocate();
	bp.allocate();
	bp.clear();
	CPPUNIT_ASSERT(!bp.active_pages().size());
	CPPUNIT_ASSERT(!bp.full_pages().size());
}

namespace{
buffer_pool_list *pool_list;
void buffer_pool_list_thread_buffer_pool_list_run(){
pool_list = &buffer_pool::pool_list();
}
}

void memory_test_class::buffer_pool_list_thread_buffer_pool_list(){
	thread t(TRACE_PARAMS(buffer_pool_list_thread_buffer_pool_list_run));
	t.join();
	CPPUNIT_ASSERT(&buffer_pool::pool_list() != pool_list);
}

void memory_test_class::buffer_pool_list_add(){
	size_t s = buffer_pool::pool_list().size();
	uint8_t buf[sizeof(buffer_pool)];
	buffer_pool *bp = new(buf)buffer_pool(10);
	CPPUNIT_ASSERT(buffer_pool::pool_list().size() == s+1);
	bp->~buffer_pool();
	CPPUNIT_ASSERT(buffer_pool::pool_list().size() == s);
}

namespace{
bool is_BF_pool_desd = false;
class BF_pool: public buffer_pool{
public:
	BF_pool(): buffer_pool(1){}
	~BF_pool(){
		is_BF_pool_desd = true;
	}
};
void BF_pool_des_run(){
	static uint8_t buf[sizeof(BF_pool)];
	new(buf)BF_pool;
}
}

void memory_test_class::buffer_pool_list_remove(){
	is_BF_pool_desd = false;
	{
		thread t(TRACE_PARAMS(BF_pool_des_run));
		t.join();
	}
	CPPUNIT_ASSERT(is_BF_pool_desd);
}

void memory_test_class::object_type_align(){
	CPPUNIT_ASSERT(&OBJECT_POOL(char)::instance() == &OBJECT_POOL(short)::instance());
}

void memory_test_class::pool_ptr_des(){
	void *p;
	{
		pool_ptr<OBJECT_POOL(int)> pptr(p = OBJECT_POOL_ALLOCATE(int));
		CPPUNIT_ASSERT(pptr.raw_pointer());
	}
	CPPUNIT_ASSERT(OBJECT_POOL(int)::instance().allocate() == p);
}

void memory_test_class::pool_ptr_des_release(){
	OBJECT_POOL(int)::instance().clear();
	void *p;
	{
		pool_ptr<OBJECT_POOL(int)> pptr(p = OBJECT_POOL_ALLOCATE(int));
		CPPUNIT_ASSERT(pptr.release() == p);
		CPPUNIT_ASSERT(!pptr.raw_pointer());
	}
	CPPUNIT_ASSERT(OBJECT_POOL(int)::instance().allocate() != p);
	OBJECT_POOL(int)::instance().clear();
}

namespace{
	class object_constructor_error{
	public:
		static bool destroyed;
		object_constructor_error(){}
		object_constructor_error(int){throw 0;}
		~object_constructor_error(){
			object_constructor_error::destroyed = true;
		}
	};
	bool object_constructor_error::destroyed = false;
}

void memory_test_class::object_ptr_des(){
	OBJECT_POOL(object_constructor_error)::instance().clear();
	object_constructor_error::destroyed = false;
	{
		object_ptr<object_constructor_error> optr(OBJECT_NEW(object_constructor_error));
		CPPUNIT_ASSERT(optr.raw_pointer());
	}
	CPPUNIT_ASSERT(OBJECT_POOL(int)::instance()._active_pages.first()->gathered());
	CPPUNIT_ASSERT(object_constructor_error::destroyed);
	OBJECT_POOL(object_constructor_error)::instance().clear();
}

void memory_test_class::object_ptr_des_release(){
	OBJECT_POOL(object_constructor_error)::instance().clear();
	object_constructor_error::destroyed = false;
	{
		object_ptr<object_constructor_error> optr(OBJECT_NEW(object_constructor_error));
		optr.release();
		CPPUNIT_ASSERT(!optr.raw_pointer());
	}
	CPPUNIT_ASSERT(!OBJECT_POOL(int)::instance()._active_pages.first()->gathered());
	CPPUNIT_ASSERT(!object_constructor_error::destroyed);
	OBJECT_POOL(object_constructor_error)::instance().clear();
}

void memory_test_class::object_ptr_construct_error(){
	OBJECT_POOL(object_constructor_error)::instance().clear();
	try{
		OBJECT_NEW(object_constructor_error, 0);
		CPPUNIT_ASSERT(!"Not throw");
	}catch(...){
	}
	CPPUNIT_ASSERT(OBJECT_POOL(int)::instance()._active_pages.first()->gathered());
	OBJECT_POOL(object_constructor_error)::instance().clear();
}

namespace{
	allocator *othda = NULL;
	void allocator_set_instance_run(){
		othda = &allocator::instance();
	}
}

void memory_test_class::allocator_set_instance(){
	thread thd(TRACE_PARAMS(allocator_set_instance_run));
	thd.join();
	CPPUNIT_ASSERT(othda!= &allocator::instance());
}

void memory_test_class::allocator_size(){
	for(size_t s = 5; s<=0x40000000; s<<=1){
		size_t *p = static_cast<size_t*>(allocator::instance().allocate(s)) - 1;
		size_t as = util::algin<size_t, sizeof(void*)>(s) + sizeof(size_t);
		size_t fs = static_cast<size_t>(MIN_USEFUL_BUFFER_SIZE)<<*p;
		CPPUNIT_ASSERT(fs >= as);
		CPPUNIT_ASSERT((fs>>1) < as);
	}
}

void memory_test_class::allocator_string(){
	const char *str = "Hello world!";
	char *str_n = allocator::instance().allocate("%s", str);
	CPPUNIT_ASSERT(!strcmp(str_n, str));
	allocator::instance().deallocate(str_n);
}

void memory_test_class::allocator_any_new(){
	object_pool<2*sizeof(size_t)>::instance().clear();
	void *p = allocator::instance().allocate(sizeof(size_t));
	CPPUNIT_ASSERT(1 == object_pool<2*sizeof(size_t)>::instance()._active_pages.first()->_allocated);
	allocator::instance().deallocate(p);
	CPPUNIT_ASSERT(object_pool<2*sizeof(size_t)>::instance()._active_pages.first()->gathered());
}

void memory_test_class::object_delete_macro(){
	des = false;
	test_obj *t = OBJECT_NEW(test_obj);
	OBJECT_DELETE(test_obj, t);
	CPPUNIT_ASSERT(des);
}

void memory_test_class::object_ptr_reassign(){
	des = false;
	object_ptr<test_obj> t(OBJECT_NEW(test_obj));
	t.reassign(NULL);
	CPPUNIT_ASSERT(des);
}