#ifndef CCLIBC_ENV_H_
#define CCLIBC_ENV_H_

#define CCLIBC cclibc
#define CCLIBC_BG namespace cclibc{
#define CCLIBC_END }

#include <stdint.h>
#include <cstdlib>

#define UNIQUE_INDENTIFIER_INNER(prefix, suffix) prefix##suffix 

#define UNIQUE_INDENTIFIER(prefix, suffix) UNIQUE_INDENTIFIER_INNER(prefix, suffix) 

CCLIBC_BG

class util{
public:
	template<typename T, T B>
	static T algin(T t) {
		return (t & (B-1)) ? (t + B - (t % B)) : t; 
	}

	template<typename T>
	static void update_min(T &v, T t){
		if(t<v){
			v=t;
		}
	}

	template<typename T>
	static void update_max(T &v, T t){
		if(t>v){
			v=t;
		}
	}
};

template<size_t S, size_t B>
class power2_algin{
public:
	enum{ result = B>=S ? B : power2_algin<S, B<<1>::result};
};

template<size_t S>
class power2_algin<S, 0>{
public:
	enum{ result = -1};
};

class object{
public:
	virtual ~object(){}
};

CCLIBC_END

#define SIZE_ALGIN(v, a) ((v) & ((a)-1) ? (((v) & (~((a)-1))) + (a)) : (v))
#define ALIGNED_SIZEOF(type)	SIZE_ALGIN(sizeof(type), sizeof(void*))


#ifdef TEST_MODE
#define private	public
#define protected public
#endif

#endif //CCLIBC_ENV_H_
