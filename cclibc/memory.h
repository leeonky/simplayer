#ifndef CCLIBC_MEMORY_H
#define CCLIBC_MEMORY_H

#include <cstdlib>
#include <memory>
#include "cclibc/env.h"
#include "cclibc/trace.h"
#include "cclibc/util/list.h"

CCLIBC_BG

class memory{
public:
	static void* malloc FUNCTION_PARAMETER_LIST(size_t s);

	static void* malloc_nothrow(size_t s){
		return ::malloc(s);
	}

	static void free(void *p){
		::free(p);
	}
};

#ifdef STACK_TRACE_FLAG

#define allocate(...) allocate(INVOKE_TRACE,## __VA_ARGS__)
#define deallocate(...) deallocate(INVOKE_TRACE,## __VA_ARGS__)
#define allocate_nothrow(...) allocate_nothrow(INVOKE_TRACE,## __VA_ARGS__)
#define retrieve(...) retrieve(INVOKE_TRACE,## __VA_ARGS__)

#endif

template<typename T>
class memory_page: public T::node {
private:
	uint8_t *_buffer;
	size_t _left;
	const size_t _size;
public:
	memory_page(uint8_t *buf, size_t len):_buffer(buf), _left(len), _size(len){}

	void* allocate_nothrow FUNCTION_PARAMETER_LIST(size_t s){
		debug_assert(enough(s));
		uint8_t *r = _buffer;
		_left -= s;
		_buffer += s;
		return r;
	}

	bool enough(size_t s) const{
		return s <= _left;
	}

	size_t left() const{
		return _left;
	}

	size_t size() const{
		return _size;
	}

	void reset() {
		_buffer -=(_size - _left);
		_left = _size;
	}
};

class stack_page: public memory_page<single_list<stack_page> > {
public:
	stack_page(uint8_t *buf, size_t len): memory_page<single_list<stack_page> >(buf, len) {}

	bool full() const {
		return left() < sizeof(void*);
	}
};

#ifndef POOL_PAGE_SIZE
#define POOL_PAGE_SIZE 4096
#endif

class stack_pool_base{
protected:
	single_list<stack_page> _active_pages;
	single_list<stack_page> _full_pages;
	size_t _page_count;
	size_t _base_page_size;

public:
	stack_pool_base(size_t pc = 1, size_t bps = POOL_PAGE_SIZE): _page_count(pc), _base_page_size(bps){}
	~stack_pool_base(){
		clear();
	}
	void* allocate FUNCTION_PARAMETER_LIST(size_t);
	char* allocate FUNCTION_PARAMETER_LIST(const char* fmt, ...);
	char* allocate FUNCTION_PARAMETER_LIST(size_t &len, const char* fmt, ...);
	void clear();
	void* allocate_nothrow FUNCTION_PARAMETER_LIST(size_t);
	char* allocate_nothrow FUNCTION_PARAMETER_LIST(const char* fmt, ...);
	void reset();
};

class stack_pool: private stack_pool_base{
private:
	static __thread stack_pool *_thread_pool;
	static stack_pool& get_thread_pool() {
		if(!_thread_pool){
			static stack_pool s;
			_thread_pool = &s;
		}
		return *_thread_pool;
	}
	
public:
	using stack_pool_base::allocate_nothrow;
	using stack_pool_base::allocate;
	using stack_pool_base::clear;
	using stack_pool_base::reset;
	
	static void set_thread_pool(stack_pool *s){
		_thread_pool = s;
	}
	
	static stack_pool& thread_pool(){
		return get_thread_pool();
	}
};

template<typename T>
class stack_ptr{
private:
	T *_p;
	stack_ptr(const stack_ptr&);
	stack_ptr& operator=(const stack_ptr&);
	
public:
	stack_ptr(T *p = NULL): _p(p) {}
	
	const T& operator*() const{
		return *_p;
	}
	
	T& operator*(){
		return *_p;
	}

	const T* operator->() const{
		return _p;
	}
	
	T* operator->(){
		return _p;
	}

	T* raw_pointer() {
		return _p;
	}

	const T* raw_pointer() const{
		return _p;
	}
	
	T* release(){
		T *r = _p;
		_p = NULL;
		return r;
	}
	
	~stack_ptr(){
		if(_p){
			_p->~T();
		}
	}
	
	void reassign(T *p){
		if(_p){
			_p->~T();
		}
		_p = p;
	}
};

#define STACK_NEW(pool, type, ...) new(pool.allocate(sizeof(type)))type(__VA_ARGS__)
#define STACK_NEW_NOTHROW(pool, type, ...) ({\
	type *_p = static_cast<type*>(pool.allocate_nothrow(sizeof(type))); \
	if(_p){\
		new(_p)type(__VA_ARGS__);\
	}\
	_p;})
	
class collect_node: public single_list<collect_node>::node{
};

class buffer_page;
struct used_node{
public:
#ifndef NDEBUG
	void *thread_id;
	void *pool;
#endif
	buffer_page *page;
};

class buffer_pool_base;
class buffer_page: public memory_page<dual_list<buffer_page> >{
private:
	single_list<collect_node> _released;
	size_t _allocated;
	buffer_pool_base &_pool;
	
public:
	buffer_page(uint8_t*, buffer_pool_base&);

	void* retrieve FUNCTION_PARAMETER_LIST();

	void deallocate FUNCTION_PARAMETER_LIST(void*);

	const single_list<collect_node>& released() const{
		return _released;
	}

	size_t allocated() const{
		return _allocated;
	}

	bool gathered() const;

	bool full() const;
};

class buffer_pool_base{
private:
	size_t _buffer_size;
	uint8_t _usage;
	size_t _page_size;
	size_t _buffer_count;

protected:
	dual_list<buffer_page> _active_pages;
	dual_list<buffer_page> _full_pages;
	
public:
	buffer_pool_base(size_t, uint8_t usage = 60, size_t bps = POOL_PAGE_SIZE);

	~buffer_pool_base(){
		clear();
	}

	size_t buffer_size() const{
		return _buffer_size;
	}

	uint8_t usage() const{
		return _usage;
	}

	size_t page_size() const{
		return _page_size;
	}

	size_t buffer_count() const{
		return _buffer_count;
	}

	void* allocate FUNCTION_PARAMETER_LIST();

	void deallocate FUNCTION_PARAMETER_LIST(void *p);

	void clear();
};

inline bool buffer_page::gathered() const{
	return !_allocated;
}

inline bool buffer_page::full() const{
	return (!_released.size()) && left() < _pool.buffer_size();
}

class buffer_pool;
class buffer_pool_list: public dual_list<buffer_pool>{
public:
	~buffer_pool_list();
};

class buffer_pool: private buffer_pool_base, private dual_list<buffer_pool>::node{
friend class dual_list<buffer_pool>;
private:
	static __thread buffer_pool_list *_thread_list;
	static buffer_pool_list& get_pool_list() {
		if(!_thread_list){
			static buffer_pool_list l;
			_thread_list = &l;
		}
		return *_thread_list;
	}
	
public:
	buffer_pool(size_t, uint8_t usage = 60, size_t bps = POOL_PAGE_SIZE);
	using buffer_pool_base::buffer_size;
	using buffer_pool_base::usage;
	using buffer_pool_base::page_size;
	using buffer_pool_base::buffer_count;
	using buffer_pool_base::allocate;
	using buffer_pool_base::deallocate;
	using buffer_pool_base::clear;
	virtual ~buffer_pool();
	
	static void set_pool_list(buffer_pool_list *l){
		_thread_list = l;
	}
	
	static buffer_pool_list& pool_list(){
		return get_pool_list();
	}
};

template<size_t TS>
class object_pool: public buffer_pool{
private:
	static __thread object_pool *_instance;
public:
	object_pool(uint8_t usage = 60, size_t bps = POOL_PAGE_SIZE): buffer_pool(TS, usage, bps){}
	static object_pool& instance(){
		if(!_instance){
			_instance = STACK_NEW(stack_pool::thread_pool(), object_pool);
		}
		return *_instance;
	}
};

template<size_t TS>
__thread object_pool<TS>* object_pool<TS>::_instance = NULL;

#define SIZE_MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN_USEFUL_BUFFER_SIZE SIZE_MAX(sizeof(CCLIBC::collect_node)>sizeof(CCLIBC::used_node) ? \
SIZE_ALGIN(sizeof(CCLIBC::collect_node)-sizeof(CCLIBC::used_node), sizeof(void*)) : 0, sizeof(void*))
#define SIZE_FOR_POOL(type)	CCLIBC::power2_algin<sizeof(type), MIN_USEFUL_BUFFER_SIZE>::result
#define OBJECT_POOL(type) CCLIBC::object_pool<SIZE_FOR_POOL(type)>

template<typename POOL>
class pool_ptr{
private:
	void *_p;
	pool_ptr(const pool_ptr&);
	pool_ptr& operator=(const pool_ptr&);

public:
	pool_ptr(void *p = NULL): _p(p){
	}

	void* raw_pointer() {
		return _p;
	}

	const void* raw_pointer() const{
		return _p;
	}
	
	operator void*() {
		return _p;
	}
	
	operator const void*() const{
		return _p;
	}


	void* release(){
		void *r = _p;
		_p = NULL;
		return r;
	}

	~pool_ptr(){
		if(_p){
			POOL::instance().deallocate(_p);
		}
	}

	void reassign(void *p){
		if(_p){
			POOL::instance().deallocate(_p);
		}
		_p = p;
	}
};

#define OBJECT_POOL_ALLOCATE(type)	OBJECT_POOL(type)::instance().allocate()
#define OBJECT_POOL_DEALLOCATE(type, p)	OBJECT_POOL(type)::instance().deallocate(p)

template<typename T, typename POOL = object_pool<SIZE_FOR_POOL(T)> >
class object_ptr: public pool_ptr<POOL>{
typedef pool_ptr<POOL> base_pool_ptr;
public:
	object_ptr(T *p = NULL): base_pool_ptr(p){} 

	const T& operator*() const{
		return *raw_pointer();
	}
	
	T& operator*(){
		return *raw_pointer();
	}

	const T* operator->() const{
		return raw_pointer();
	}
	
	T* operator->(){
		return raw_pointer();
	}

	T* raw_pointer() {
		return static_cast<T*>(base_pool_ptr::raw_pointer());
	}

	const T* raw_pointer() const{
		return static_cast<const T*>(base_pool_ptr::raw_pointer());
	}
	
	operator T*() {
		return raw_pointer();
	}
	
	operator const T*() const{
		return raw_pointer();
	}
	
	T* release(){
		base_pool_ptr::release();
		return raw_pointer();
	}
	
	~object_ptr(){
		if(raw_pointer()){
			raw_pointer()->~T();
		}
	}

	void reassign(T *p){
		if(raw_pointer()){
			raw_pointer()->~T();
		}
		base_pool_ptr::reassign(p);
	}
};

#define OBJECT_NEW(type, ...)	({CCLIBC::pool_ptr<OBJECT_POOL(type)> _tmp_pool_ptr(OBJECT_POOL_ALLOCATE(type)); \
new (_tmp_pool_ptr.raw_pointer())type(__VA_ARGS__); static_cast<type*>(_tmp_pool_ptr.release());})

#define OBJECT_DELETE(type, p) do{if(p){\
	p->~type();\
	OBJECT_POOL_DEALLOCATE(type, p);\
	p = NULL;\
}}while(false)

#ifndef MAX_ALLOCATE_EXPONENT
#define MAX_ALLOCATE_EXPONENT	29
#endif

class allocator{
private:
	static __thread allocator *_instance;
	buffer_pool* _pools[MAX_ALLOCATE_EXPONENT];
public:
	allocator();
	void* allocate FUNCTION_PARAMETER_LIST(size_t);
	char* allocate FUNCTION_PARAMETER_LIST(const char*, ...);
	void deallocate FUNCTION_PARAMETER_LIST(void*);

	static void set_instance(allocator *a){
		_instance = a;
	}

	static allocator& instance(){
		if(!_instance){
			static allocator a;
			_instance = &a;
		}
		return *_instance;
	}
};

#define ALLOCATOR_NEW(type, ...)	({CCLIBC::pool_ptr<CCLIBC::allocator> _tmp_pool_ptr(CCLIBC::allocator::instance().allocate(sizeof(type))); \
new (_tmp_pool_ptr.raw_pointer())type(__VA_ARGS__); static_cast<type*>(_tmp_pool_ptr.release());})

template<typename T>
void ALLOCATOR_DELETE(T *&p) {
	if(p){
		p->~T();
		allocator::deallocate(p);
		p = NULL;
	}
}
/*
class global_allocator{
private:
	allocator _allocator;
	static global_allocator _instance;
public:
	void* allocate FUNCTION_PARAMETER_LIST(size_t);
	char* allocate FUNCTION_PARAMETER_LIST(const char*, ...);
	void deallocate FUNCTION_PARAMETER_LIST(void*);
	static global_allocator& instance(){
		return _instance;
	}
};
*/
/*
template<typename T, typename L>
void delete_dual_list_elements(L &l){
	T *n = l.last();
	l.discard();
	while(n){
		T *pn = n->previous();
		n->~T();
		object_pool<SIZE_FOR_POOL(T)>::instance().deallocate(n);
		n = pn;
	}
}

template<typename T, typename POOL, typename L>
void delete_dual_list_elements(L &l){
	T *n = l.last();
	l.discard();
	while(n){
		T *pn = n->previous();
		n->~T();
		POOL::instance().deallocate(n);
		n = pn;
	}
}
template<typename T, typename POOL, typename L>
void delete_single_list_elements(L &l){
	T *n = l.last();
	l.discard();
	while(n){
		T *nn = n->next();
		n->~T();
		POOL::instance().deallocate(n);
		n = nn;
	}
}
*/

template<typename T, typename L>
void delete_single_list_elements(L &l){
	T *n = l.first();
	l.discard();
	while(n){
		T *nn = n->next();
		OBJECT_DELETE(T, n);
		n = nn;
	}
}

template<typename N>
void destruct_single_list_elements(single_list<N> &l){
	N *n = l.first();
	l.discard();
	while(n){
		N *nn = n->next();
		n->~N();
		n = nn;
	}
}


CCLIBC_END

#endif //CCLIBC_MEMORY_H
