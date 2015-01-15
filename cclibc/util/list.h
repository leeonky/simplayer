#ifndef CCLIBC_UTIL_NODE_LIST_H_
#define CCLIBC_UTIL_NODE_LIST_H_

#include <utility>
#include "cclibc/env.h"
#include "cclibc/assert.h"
#include "cclibc/trace.h"

#ifdef STACK_TRACE_FLAG

#ifndef add
#define add(...) add(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#ifndef remove
#define remove(...) remove(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#define insert_before(...) insert_before(INVOKE_TRACE, ##__VA_ARGS__)
#define insert_after(...) insert_after(INVOKE_TRACE, ##__VA_ARGS__)
#define remove_first(...) remove_first(INVOKE_TRACE, ##__VA_ARGS__)
#define remove_next(...) remove_next(INVOKE_TRACE, ##__VA_ARGS__)
#define replace(...) replace(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCLIBC_BG

template<typename T>
class dual_list{
private:
	dual_list(const dual_list&);
	dual_list& operator=(const dual_list&);
public:
	class node{
	friend class dual_list;
	private:
		T *_next;
		T *_prev;
#ifndef NDEBUG
		void *_list;
#endif
	protected:
		node(): _next(NULL), _prev(NULL)
#ifndef NDEBUG
			, _list(NULL)
#endif
		{}
	public:
		const T* next() const{
			return _next;
		}

		T* next(){
			return _next;
		}

		const T* previous() const{
			return _prev;
		}

		T* previous(){
			return _prev;
		}

#ifndef NDEBUG
		void* list(){
			return _list;
		}

		const void* list() const{
			return _list;
		}
#endif
	};

private:
	T *_first;
	T *_last;
	size_t _size;
public:
	dual_list(): _first(NULL), _last(NULL), _size(0) {}
	
	~dual_list(){
		clear();
	}
	
	const T* first() const {
		return _first;
	}

	T* first(){
		return _first;
	}

	const T* last() const {
		return _last;
	}

	T* last(){
		return _last;
	}

	size_t size() const {
		return _size;
	}

	void add FUNCTION_PARAMETER_LIST(T *e) {
		debug_assert(e);
		debug_assert(!(e->next() || e->previous()));
#ifndef NDEBUG
		debug_assert(!e->list());
#endif
		if(_size){
			e->_prev = _last;
			_last->_next = e;
		}
		else{
			_first = e;
		}
		_last = e;
		++_size;
#ifndef NDEBUG
		e->_list = this;
#endif
	}

	T* remove FUNCTION_PARAMETER_LIST(T *e){
		debug_assert(_size);
		debug_assert(e);
#ifndef NDEBUG
		debug_assert(e->list() == this);
#endif
		if(e->_next){
			e->_next->_prev = e->_prev;
		}
		if(e->_prev){
			e->_prev->_next = e->_next;
		}
		if(_first == e){
			_first = e->_next;
			e->_next = NULL;
		}
		if(_last == e){
			_last = e->_prev;
			e->_prev = NULL;
		}
		e->_prev = e->_next = NULL;
#ifndef NDEBUG
		e->_list = NULL;
#endif
		--_size;
		return e;
	}

	void insert_before FUNCTION_PARAMETER_LIST(T *e, T *w){
		debug_assert(e);
		debug_assert(!(e->next() || e->previous()));
#ifndef NDEBUG
		debug_assert(!e->list());
#endif
		if(w){
#ifndef NDEBUG
			debug_assert(w->list() == this);
#endif
			if(w == _first){
				_first = e;
			}else{
				e->_prev = w->_prev;
				e->_prev->_next = e;
			}
			e->_next = w;
			w->_prev = e;
		}else{
			if(_size){
				e->_prev = _last;
				_last->_next = e;
			}
			else{
				_first = e;
			}
			_last = e;
		}
		++_size;
#ifndef NDEBUG
		e->_list = this;
#endif
	}

	void insert_after FUNCTION_PARAMETER_LIST(T *e, T *w) {
		debug_assert(e);
		debug_assert(!(e->next() || e->previous()));
#ifndef NDEBUG	
		debug_assert(!e->list());
#endif
		if(w){
#ifndef NDEBUG	
			debug_assert(w->list() == this);
#endif
			if(w == _last){
				_last = e;
			}else{
				e->_next = w->_next;
				e->_next->_prev = e;
			}
			e->_prev = w;
			w->_next = e;
		}
		else{
			if(_size){
				_first->_prev = e;
				e->_next = _first;
			}else{
				_last = e;
			}
			_first = e;
		}
		++_size;
#ifndef NDEBUG
		e->_list = this;
#endif
	}

	T* replace FUNCTION_PARAMETER_LIST(T *f, T *t){
		debug_assert(!t->next());
		debug_assert(!t->previous());
#ifndef NDEBUG
		debug_assert(this == f->list());
		debug_assert(!t->list());
#endif
		if(f->_next){
			std::swap(f->_next, t->_next);
			t->_next->_prev = t;
		}else{
			_last = t;
		}
		if(f->_prev){
			std::swap(f->_prev, t->_prev);
			t->_prev->_next = t;
		}else{
			_first = t;
		}
#ifndef NDEBUG
		f->_list = NULL;
		t->_list = this;
#endif
		return f;
	}
	
	void clear(){
		T *n = _first;
		while(n){
			T *p = n;
			n = n->_next;
			p->_next = p->_prev = NULL;
#ifndef NDEBUG
			p->_list = NULL;
#endif
		}
		_size = 0;
		_first = _last = NULL;
	}

	void discard(){
#ifndef NDEBUG
		T *n = _first;
		while(n){
			n->_list = NULL;
			n = n->_next;
		}
#endif
		_size = 0;
		_first = _last = NULL;
	}
};

template<typename T>
class single_list{
private:
	T *_first;
	T *_last;
	size_t _size;
public:
	class node{
	friend class single_list;
	private:
		T *_next;
#ifndef NDEBUG
		void *_list;
#endif
	public:
		node(): _next(NULL)
#ifndef NDEBUG
	, _list(NULL)
#endif
		{}

		T* next(){
			return _next;
		}

		const T* next() const{
			return _next;
		}

#ifndef NDEBUG
		void* list(){
			return _list;
		}
		const void* list() const{
			return _list;
		}
#endif
	};

	single_list(): _first(NULL), _last(NULL), _size(0){}

	~single_list(){
		clear();
	}

	T* first(){
		return _first;
	}
	
	const T* first() const{
		return _first;
	}

	T* last(){
		return _last;
	}
	
	const T* last() const{
		return _last;
	}

	size_t size() const{
		return _size;
	}

	void add FUNCTION_PARAMETER_LIST(T *e) {
		debug_assert(e);
		debug_assert(!e->next());
#ifndef NDEBUG
		debug_assert(!e->list());
#endif
		if(_size){
			_last->_next = e;
		}else{
			_first = e;
		}
		_last = e;
		++_size;
#ifndef NDEBUG
		e->_list = this;
#endif
	}

	T* remove FUNCTION_PARAMETER_LIST(T *e){
		debug_assert(e);
		debug_assert(_size);
#ifndef NDEBUG
		debug_assert(e->list() == this);
#endif
		if(_first == e){
			if(_last == e){
				_first = _last = NULL;
			}else{
				_first = e->_next;
				e->_next = NULL;
			}
		}else{
			T *be=_first;
			while(be && be->_next != e){
				be = be->_next;
			}
			debug_assert(be);
			if(_last == e){
				_last = be;
				be->_next = NULL;
			}else{
				be->_next = e->_next;
				e->_next = NULL;
			}
		}
		--_size;
#ifndef NDEBUG
		e->_list = NULL;
#endif
		return e;
	}

	T* remove_first FUNCTION_PARAMETER_LIST(){
		debug_assert(_size);
		T *e = _first;
		if(_last == e){
			_first = _last = NULL;
		}else{
			_first = e->_next;
			e->_next = NULL;
		}
		--_size;
#ifndef NDEBUG
		e->_list = NULL;
#endif
		return e;
	}

	T* remove_next FUNCTION_PARAMETER_LIST(T *pre){
		T *e;
		if(!pre){
			e = _first;
			if(_last == e){
				_first = _last = NULL;
			}else{
				_first = e->_next;
				e->_next = NULL;
			}
		}else{
			e = pre->_next;
			if(_last == e){
				_last = pre;
				pre->_next = NULL;
			}else if(e){
				pre->_next = e->_next;
				e->_next = NULL;
			}else{
				debug_assert(!"Pass last to remove_last!");
			}
		}
		--_size;
#ifndef NDEBUG
		e->_list = NULL;
#endif
		return e;
	}
	
	void clear(){
		T *n = _first;
		while(n){
			T *p = n;
			n = n->_next;
			p->_next = NULL;
#ifndef NDEBUG
			p->_list = NULL;
#endif
		}
		_size = 0;
		_first = _last = NULL;
	}

	void discard(){
#ifndef NDEBUG
		T *n = _first;
		while(n){
			n->_list = NULL;
			n = n->_next;
		}
#endif
		_size = 0;
		_first = _last = NULL;
	}
};

CCLIBC_END
	
#define LIST_FOREACH(type,var,list_r) for(type *_next_##var = NULL,*var = (list_r).first();\
	(var ? (_next_##var = var ->next(),true):false);\
	var =_next_##var )

#define LIST_FOREACH_R(type,var,list_r) for(type *_next_##var = NULL,*var = (list_r).last();\
	(var ? (_next_##var = var ->previous(),true):false);\
	var =_next_##var )

#define SLIST_FOREACH_NOLAST(type, var1, var2, slist_r) for(type *var1 = NULL,*var2 = (slist_r).first();\
	var1 != (slist_r).last();	var1 = var2, var2 = var2->next() )


#endif //CCLIBC_UTIL_dual_list_H_

