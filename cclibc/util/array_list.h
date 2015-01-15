#ifndef CCLIBC_UTIL_ARRAY_LIST_H_
#define CCLIBC_UTIL_ARRAY_LIST_H_

#include "cclibc/assert.h"
#include "cclibc/trace.h"
#include "cclibc/memory.h"


#ifdef STACK_TRACE_FLAG

#ifndef add
#define add(...) add(INVOKE_TRACE, ##__VA_ARGS__)
#endif
#define to_array(...) to_array(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCLIBC_BG

template<typename T, typename POOL=allocator>
class array_list{
private:
	size_t _size;
	size_t _cap;
	T *_data;
public:
	array_list(): _size(0), _cap(0), _data(NULL){}

	size_t size() const{
		return _size;
	}
	size_t capcity() const{
		return _cap;
	}

	void add FUNCTION_PARAMETER_LIST(const T &v){
		if(_cap < _size + 1){
			if(_cap){
				_cap <<= 1;
				T *buf = static_cast<T*>(POOL::instance().allocate(sizeof(T) * _cap));
				memcpy(buf, _data, sizeof(T)*_size);
				POOL::instance().deallocate(_data);
				_data =  buf;
			}else{
				_cap = 1;
				_data = static_cast<T*>(POOL::instance().allocate(sizeof(T)));
			}
		}
		new(_data + _size)T(v);
		++_size;
	}

	T& operator[](size_t i){
		return const_cast<T&>(const_cast<const array_list*>(this)->operator[](i));
	}

	const T& operator[](size_t i) const{
		debug_assert(i<size());
		return _data[i];
	}

	void clear(){
		for(int i=_size-1; i>=0; --i){
			_data[i].~T();
		}
		POOL::instance().deallocate(_data);
		_data = NULL;
		_size = 0;
		_cap = 0;
	}
	
	~array_list(){
		clear();
	}
	
	template<typename P>
	T* to_array FUNCTION_PARAMETER_LIST(){
		size_t len = _size * sizeof(T);
		T* res = static_cast<T*>(P::instance().allocate(len));
		memcpy(res, _data, len);
		return res;
	}
};

CCLIBC_END

#endif //CCLIBC_UTIL_ARRAY_LIST_H_

