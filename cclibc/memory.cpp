#include <memory>
#include <cstdarg>
#include "cclibc/memory.h"
#include "cclibc/exception.h"
//#include "cclibc/thread/mutex.h"

#ifdef STACK_TRACE_FLAG

#define malloc(...) malloc(INVOKE_TRACE,## __VA_ARGS__)

#endif

CCLIBC_BG

void* memory::malloc FUNCTION_PARAMETER_LIST(size_t s){
	void * r = malloc_nothrow(s);
	if(r){
		return r;
	}
	THROW_NEW_EXCEPTION(allocate_error);
}

void stack_pool_base::clear(){
	stack_page *p,*n;
	p = _active_pages.first();
	_active_pages.discard();
	while(p){
		n = p->next();
		p->~stack_page();
		memory::free(p);
		p = n;
	}
	p = _full_pages.first();
	_full_pages.discard();
	while(p){
		n = p->next();
		p->~stack_page();
		memory::free(p);
		p = n;
	}
}

void* stack_pool_base::allocate_nothrow FUNCTION_PARAMETER_LIST(size_t s){
	void *r = NULL;
	s = util::algin<size_t, sizeof(void*)>(s);
	SLIST_FOREACH_NOLAST(stack_page, pre_page, page, _active_pages){
		if(page->enough(s)){
			r = page->allocate_nothrow(s);
			if(page->full()){
				_full_pages.add(_active_pages.remove_next(pre_page));
			}
			return r;
		}
	}

	size_t base_size = _page_count * _base_page_size;
	while(base_size < ALIGNED_SIZEOF(stack_page) + s){
		base_size += _page_count * _base_page_size;
	}
	uint8_t *p = static_cast<uint8_t*>(memory::malloc_nothrow(base_size));
	if(p){
		stack_page *tp = new(p)stack_page(p + ALIGNED_SIZEOF(stack_page), base_size - ALIGNED_SIZEOF(stack_page));
		r = tp->allocate_nothrow(s);
		if(tp->full()){
			_full_pages.add(tp);
		}else{
			_active_pages.add(tp);
		}
	}
	return r;
}

char* stack_pool_base::allocate_nothrow FUNCTION_PARAMETER_LIST(const char *fmt, ...){
	va_list list;
	va_start(list, fmt);
	int len = vsnprintf(NULL, 0, fmt, list);
	va_end(list);
	debug_assert(len >= 0);
	char *buf = static_cast<char*>(allocate_nothrow(static_cast<size_t>(++len)));
	if(buf){
		va_start(list, fmt);
		debug_assert(vsnprintf(buf, len, fmt, list)>=0);
		va_end(list);
	}
	return buf;
}

void stack_pool_base::reset(){
	if(_active_pages.first()){
		stack_page *p = _active_pages.remove_first();
		p->reset();
		clear();
		_active_pages.add(p);
	}else{
		if(_full_pages.first()){
			_active_pages.add(_full_pages.remove_first());
			reset();
		}
	}
}

void* stack_pool_base::allocate FUNCTION_PARAMETER_LIST(size_t s){
	void * p = allocate_nothrow(s);
	if(p){
		return p;
	}else{
		THROW_NEW_EXCEPTION(allocate_error);
	}
}

char* stack_pool_base::allocate FUNCTION_PARAMETER_LIST(const char* fmt, ...){
	va_list list;
	va_start(list, fmt);
	int len = vsnprintf(NULL, 0, fmt, list);
	va_end(list);
	debug_assert(len >= 0);
	char *buf = static_cast<char*>(allocate_nothrow(static_cast<size_t>(++len)));
	if(buf){
		va_start(list, fmt);
		debug_assert(vsnprintf(buf, len, fmt, list)>=0);
		va_end(list);
	}else{
		THROW_NEW_EXCEPTION(allocate_error);
	}
	return buf;
}

char* stack_pool_base::allocate FUNCTION_PARAMETER_LIST(size_t &len, const char* fmt, ...){
	va_list list;
	va_start(list, fmt);
	len = vsnprintf(NULL, 0, fmt, list);
	va_end(list);
	debug_assert((int)len >= 0);
	char *buf = static_cast<char*>(allocate_nothrow(static_cast<size_t>(++len)));
	if(buf){
		va_start(list, fmt);
		debug_assert(vsnprintf(buf, len, fmt, list)>=0);
		va_end(list);
	}else{
		THROW_NEW_EXCEPTION(allocate_error);
	}
	--len;
	return buf;
}

__thread stack_pool* stack_pool::_thread_pool = NULL;

buffer_page::buffer_page(uint8_t *buf, buffer_pool_base &pool): memory_page<dual_list<buffer_page> >(buf, pool.buffer_size() * pool.buffer_count()), _allocated(0), _pool(pool) {}

void* buffer_page::retrieve FUNCTION_PARAMETER_LIST(){
	void *r;
	if(_released.size()){
		collect_node *n = _released.remove_first();
		n->~collect_node();
		r = n;
	}else{
		r = allocate_nothrow(_pool.buffer_size());
	}
	if(r){
		++_allocated;
		used_node *u = static_cast<used_node*>(r);
		u->page = this;
#ifndef NDEBUG
		u->thread_id = &stack_pool::thread_pool();
		u->pool = &_pool;
#endif
		return static_cast<uint8_t*>(r) + ALIGNED_SIZEOF(used_node);
	}
	THROW_NEW_EXCEPTION(allocate_error);
}

void buffer_page::deallocate FUNCTION_PARAMETER_LIST(void *p) {
	if(p){
		p = static_cast<uint8_t*>(p) - ALIGNED_SIZEOF(used_node);
#ifndef NDEBUG
		debug_assert(static_cast<used_node*>(p)->thread_id == &stack_pool::thread_pool());
#endif
		_released.add(new(p)collect_node());
		--_allocated;
	}
}

buffer_pool_base::buffer_pool_base(size_t bs, uint8_t usage, size_t bps): _buffer_size(std::max(util::algin<size_t,sizeof(void*)>(bs) + ALIGNED_SIZEOF(used_node), ALIGNED_SIZEOF(collect_node))), _usage(usage), _page_size(bps){
	debug_assert(bs);
	debug_assert(usage<100);
	debug_assert(bs);
	size_t ps = (_buffer_size + ALIGNED_SIZEOF(buffer_page) > bps) ? 
		(_buffer_size + ALIGNED_SIZEOF(buffer_page) + bps - 1)/bps * bps : bps;
	
	while((_buffer_count = (ps - ALIGNED_SIZEOF(buffer_page))/_buffer_size) * static_cast<uint64_t>(_buffer_size) * 100 / ps < _usage && _usage){
		ps += bps;
	}
	_page_size = ps;
}

void* buffer_pool_base::allocate FUNCTION_PARAMETER_LIST(){
	void *r;
	if(_active_pages.size()){
		buffer_page *p = _active_pages.first();
		r = p->retrieve();
		if(p->full()){
			_full_pages.add(_active_pages.remove(p));
		}
	}else{
		uint8_t *buf = static_cast<uint8_t*>(memory::malloc(_page_size));
		buffer_page *p = new(buf)buffer_page(buf + ALIGNED_SIZEOF(buffer_page), *this);
		r = p->retrieve();
		if(p->full()){
			_full_pages.add(p);
		}else{
			_active_pages.add(p);
		}
	}
	if(r){
		return r;
	}
	THROW_NEW_EXCEPTION(allocate_error);
}

void buffer_pool_base::deallocate FUNCTION_PARAMETER_LIST(void *p){
	if(p){
		debug_assert(!((sizeof(void*)-1) & reinterpret_cast<uintptr_t>(p)));
		void *up = static_cast<uint8_t*>(p) - ALIGNED_SIZEOF(used_node);
#ifndef NDEBUG
		debug_assert(static_cast<used_node*>(up)->pool == this);
#endif
		buffer_page *pg = static_cast<used_node*>(up)->page;
		if(pg->full()){
			_active_pages.add(_full_pages.remove(pg));
		}
		pg->deallocate(p);
		if(_active_pages.size() > 1 && pg->gathered()){
			_active_pages.remove(pg);
			pg->~buffer_page();
			memory::free(pg);
			pg = NULL;
		}
	}
}

void buffer_pool_base::clear(){

	buffer_page *p,*n;
	p = _active_pages.first();
	_active_pages.discard();
	while(p){
		n = p->next();
		p->~buffer_page();
		memory::free(p);
		p = n;
	}
	p = _full_pages.first();
	_full_pages.discard();
	while(p){
		n = p->next();
		p->~buffer_page();
		memory::free(p);
		p = n;
	}
}

__thread buffer_pool_list* buffer_pool::_thread_list = NULL;

buffer_pool_list::~buffer_pool_list(){
	while(buffer_pool::pool_list().size()){
		buffer_pool::pool_list().first()->~buffer_pool();
	}
}

buffer_pool::buffer_pool(size_t s, uint8_t usage, size_t bps): buffer_pool_base(s, usage, bps){
	buffer_pool::pool_list().add(this);
};

buffer_pool::~buffer_pool(){
	buffer_pool::pool_list().remove(this);
}

__thread allocator* allocator::_instance = NULL;

namespace{
	template<size_t S>
	class allocator_pools_settor
	{
	public:
		static void set_pools(buffer_pool* (&_pools)[MAX_ALLOCATE_EXPONENT]){
			_pools[S-1] = &object_pool<static_cast<size_t>(MIN_USEFUL_BUFFER_SIZE)<<(S-1)>::instance();
			allocator_pools_settor<S-1>::set_pools(_pools);
		}
	};
	template<>
	class allocator_pools_settor<1>
	{
	public:
		static void set_pools(buffer_pool* (&_pools)[MAX_ALLOCATE_EXPONENT]){
			_pools[0] = &object_pool<MIN_USEFUL_BUFFER_SIZE>::instance();
		}
	};
}

allocator::allocator(){
	allocator_pools_settor<MAX_ALLOCATE_EXPONENT>::set_pools(_pools);
}

void* allocator::allocate FUNCTION_PARAMETER_LIST(size_t l){
	unsigned n = (l+MIN_USEFUL_BUFFER_SIZE-1 + sizeof(size_t))/MIN_USEFUL_BUFFER_SIZE;
	if(1 == __builtin_popcount(n)){
		n = __builtin_ctz(n);
	}else{
		n = sizeof(n)*8 - __builtin_clz(n);
	}
	debug_assert(n<MAX_ALLOCATE_EXPONENT);
	size_t *p = static_cast<size_t*>(_pools[n]->allocate());
	*p = n;
	return p+1;
}

char* allocator::allocate FUNCTION_PARAMETER_LIST(const char *fmt, ...){
	va_list list;
	va_start(list, fmt);
	int len = vsnprintf(NULL, 0, fmt, list);
	va_end(list);
	debug_assert(len >= 0);
	char *buf = static_cast<char*>(allocate(static_cast<size_t>(++len)));
	va_start(list, fmt);
	debug_assert(vsnprintf(buf, len, fmt, list)>=0);
	va_end(list);
	return buf;
}

void allocator::deallocate FUNCTION_PARAMETER_LIST(void *p){
	if(p){
		size_t *sp = static_cast<size_t*>(p) - 1;
		debug_assert(*sp<MAX_ALLOCATE_EXPONENT);
		_pools[*sp]->deallocate(sp);
	}
}
/*
namespace{
	mutex TRACE_CONSTRUCT(_g_a_mutex);
}
global_allocator global_allocator::_instance;

void* global_allocator::allocate FUNCTION_PARAMETER_LIST(size_t l){
	GUARD_LOCK(_g_a_mutex);
	return _instance._allocator.allocate(l);
}

char* global_allocator::allocate FUNCTION_PARAMETER_LIST(const char *fmt, ...){
	GUARD_LOCK(_g_a_mutex);
	va_list list;
	va_start(list, fmt);
	int len = vsnprintf(NULL, 0, fmt, list);
	va_end(list);
	debug_assert(len >= 0);
	char *buf = static_cast<char*>(allocate(static_cast<size_t>(++len)));
	va_start(list, fmt);
	debug_assert(vsnprintf(buf, len, fmt, list)>=0);
	va_end(list);
	return buf;
}
void global_allocator::deallocate FUNCTION_PARAMETER_LIST(void *p){
	GUARD_LOCK(_g_a_mutex);
	_instance._allocator.deallocate(p);
}
*/
CCLIBC_END
