#ifndef CCLIBC_UTIL_MAP_H_
#define CCLIBC_UTIL_MAP_H_

#include "cclibc/env.h"
#include "cclibc/util/avl_tree.h"
#include "cclibc/memory.h"
#include "cclibc/exception.h"

CCLIBC_BG

template<typename K, typename V, typename C = comparer<K>, template<size_t> class POOL = object_pool>
class map{
private:
	typedef avl_tree<K, V, C> map_tree;
	map_tree _tree;
public:
	class entry: private map_tree::node{
	friend class map;
	public:
		entry(const K &k, const V &v): map_tree::node(k, v){}

		const entry* previous() const {
			return static_cast<const entry*>(map_tree::node::previous());
		}

		entry* previous(){
			return static_cast<entry*>(map_tree::node::previous());
		}

		const entry* next() const {
			return static_cast<const entry*>(map_tree::node::next());
		}

		entry* next(){
			return static_cast<entry*>(map_tree::node::next());
		}
		
		const K& key() const{
			return map_tree::node::value();
		}
		
		const V& value() const{
			return map_tree::node::payload();
		}
		
		V& value(){
			return map_tree::node::payload();
		}
	};
	typedef POOL<SIZE_FOR_POOL(entry)> allocator_type;
	typedef entry entry_type;

	~map(){
		clear();
	}

	V& operator[](const K &k){
		typename map_tree::node *f;
		if(int c = _tree.find(f, k)){
			entry *e = new(allocator_type::instance().allocate())entry(k, V());
			_tree.add(f, c, e);
			return e->payload();
		}else{
			return f->payload();
		}
	}
	const V& operator[](const K &k) const{
		const typename map_tree::node *f;
		if(_tree.find(f, k)){
			THROW_ILLEGAL_ARGUMENT("No such key.", "k");
		}else{
			return f->payload();
		}
	}

	void add FUNCTION_PARAMETER_LIST(const K &k, const V &v){
		typename map_tree::node *f;
		if(int c = _tree.find(f, k)){
			_tree.add(f, c, new(allocator_type::instance().allocate())entry(k, v));
		}else{
			f->payload() = v;
		}
	}

	const V& get(const K &k) const{
		return (*this)[k];
	}
	
	entry* get_entry(const K &k){
		typename map_tree::node *f;
		int c = _tree.find(f, k);
		if(!c){
			return static_cast<entry*>(f);
		}
		return NULL;
	}
	
	const entry* get_entry(const K &k) const{
		const typename map_tree::node *f;
		int c = _tree.find(f, k);
		if(!c){
			return static_cast<const entry*>(f);
		}else{
			return NULL;
		}
	}
	
	size_t size() const{
		return _tree.size();
	}

	bool contains(const K &k) const{
		const typename map_tree::node *f;
		return !_tree.find(f, k);
	}

	entry* first() {
		return static_cast<entry*>(_tree.first());
	}
	const entry* first() const{
		return static_cast<const entry*>(const_cast<const map_tree&>(_tree).first());
	}

	entry* last() {
		return static_cast<entry*>(_tree.last());
	}

	const entry* last() const{
		return static_cast<const entry*>(const_cast<const map_tree&>(_tree).last());
	}

	void clear(){
		entry *e = last();
		_tree.discard();
		while(e){
			entry *ne = e->previous();
			e->~entry();
			allocator_type::instance().deallocate(e);
			e = ne;
		}
	}
	
	V remove FUNCTION_PARAMETER_LIST(const K &k){
		typename map_tree::node *f = _tree.remove(k);;
		if(f){
			V v = f->payload();
			allocator_type::instance().deallocate(f);
			return v;
		}else{
			THROW_ILLEGAL_ARGUMENT("No such key.", "k");
		}
	}
	
	void erase(const K &k){
		allocator_type::instance().deallocate(_tree.remove(k));
	}
};

CCLIBC_END

#endif //CCLIBC_UTIL_MAP_H_
