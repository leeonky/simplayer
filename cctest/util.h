
#ifndef CCTEST_UTIL_H_
#define CCTEST_UTIL_H_

#include <ctime>
#include <cstring>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "cclibc/env.h"
#include "cclibc/trace.h"

#ifndef DO_COMPILE
#define	CPPUNIT_TEST_SUITE(a)	int	_x_##a
#define	CPPUNIT_TEST(a)	int _y_##a
#define	CPPUNIT_TEST_SUITE_END	int _z
#endif

#define CPPUNIT_ASSERT_EXT(exp,fmt, ...)	do{\
bool f = exp;\
	if(!f){\
		printf(fmt, ##__VA_ARGS__);\
CPPUNIT_ASSERT(!#exp);\
}\
	}while(0)

class TestTimer{
	timeval tst;
	const char *name;
public:
	TestTimer(const char *f, int l, const char *n);
	~TestTimer();
};

#define TIMER_ON(name)	TestTimer UNIQUE_INDENTIFIER(_test, __COUNTER__)(__FILE__, __LINE__, name)

class method_parameter{
private:
	uint8_t data[8];
	std::string str_value;
public:
	method_parameter(){}
	
	template<typename T>
	method_parameter(T t){
		bzero(data, sizeof(data));
		memcpy(data, &t, sizeof(T));
	}
	
	method_parameter(const char *str){
		if(str){
			str_value = str;
		}else{
			str_value = "(__null)";
		}
		bzero(data, sizeof(data));
		memcpy(data, &str, sizeof(const char*));
	}
	
	method_parameter(char *str){
		if(str){
			str_value = str;
		}else{
			str_value = "(__null)";
		}
		bzero(data, sizeof(data));
		memcpy(data, &str, sizeof(char*));
	}
	
	template<typename T>
	void set_parameter(T t){
		bzero(data, sizeof(data));
		memcpy(data, &t, sizeof(T));
	}

	bool equal(method_parameter &t){
		return !memcmp(t.data, data, sizeof(data));
	}
	
	template<typename T>
	bool equal(T t){
		return !memcmp(&t, data, sizeof(T));
	}
	
	bool equal(const char *str){
		if(str){
			return str_value == str;
		}else{
			return str_value == "(__null)";
		}
	}

	bool equal(char *str){
		if(str){
			return str_value == str;
		}else{
			return str_value == "(__null)";
		}
	}
	
	template<typename T>
	void get_value(T& t){
		memcpy(&t, data, sizeof(T));
	}
};

class method_invoke{
private:
	const char* _function;
	std::vector<method_parameter> _parameters;
	method_parameter _return;

public:
	method_invoke(const char *fun): _function(fun){}
	
	bool equal(const char *f){
		return !strcmp(f, _function);
	}
	method_parameter& parameter(size_t i) {
		return _parameters.at(i);
	}
	
	template<typename T>
	void push(T t){
		_parameters.push_back(method_parameter(t));
	}

	void push(const char *t){
		_parameters.push_back(method_parameter(t));
	}

	void push(char *t){
		_parameters.push_back(method_parameter(t));
	}
	
	
	template<typename T>
	T set_return(T t){
		_return.set_parameter(t);
		return t;
	}
	
	method_parameter get_return(){
		return _return;
	}
	
	const char* name() const{
		return _function;
	}
};

class interface_testor{
private:
	static std::vector<method_invoke> _methods;

public:
	static bool record_stack;
	static void clear(){
		_methods.clear();
	}
	static method_invoke& method(size_t i){
		return _methods.at(i);
	}
	static method_invoke& last_method(size_t i){
		return _methods.at(_methods.size() - i - 1);
	}
	static void push(const method_invoke& m){
		if(record_stack){
			_methods.push_back(m);
		}
	}
};

#define INTERFACE_TESTABLE_VOID_FUNCTION(call, realcall) \
void call(void){ \
	method_invoke _mth_ivk(#call); \
	interface_testor::push(_mth_ivk); \
	realcall(); \
}

#define INTERFACE_TESTABLE_VOID_FUNCTION_1(call, realcall, arg_type1) \
void call(arg_type1 a1){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	interface_testor::push(_mth_ivk); \
	realcall(a1); \
}

#define INTERFACE_TESTABLE_VOID_FUNCTION_2(call, realcall, arg_type1, arg_type2) \
void call(arg_type1 a1, arg_type2 a2){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	interface_testor::push(_mth_ivk); \
	realcall(a1, a2); \
}

#define INTERFACE_TESTABLE_VOID_FUNCTION_3(call, realcall, arg_type1, arg_type2, arg_type3) \
void call(arg_type1 a1, arg_type2 a2, arg_type3 a3){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	interface_testor::push(_mth_ivk); \
	realcall(a1, a2, a3); \
}

#define INTERFACE_TESTABLE_VOID_FUNCTION_4(call, realcall, arg_type1, arg_type2, arg_type3, arg_type4) \
void call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	interface_testor::push(_mth_ivk); \
	realcall(a1, a2, a3, a4); \
}

#define INTERFACE_TESTABLE_VOID_FUNCTION_5(call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5) \
void call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	interface_testor::push(_mth_ivk); \
	realcall(a1, a2, a3, a4, a5); \
}

#define INTERFACE_TESTABLE_VOID_FUNCTION_6(call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5, arg_type6) \
void call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5, arg_type6 a6){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	_mth_ivk.push(a6); \
	interface_testor::push(_mth_ivk); \
	realcall(a1, a2, a3, a4, a5, a6); \
}

#define INTERFACE_TESTABLE_FUNCTION(rtype, call, realcall) \
rtype call(void){ \
	method_invoke _mth_ivk(#call); \
	rtype ret = realcall(); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_1(rtype, call, realcall, arg_type1) \
rtype call(arg_type1 a1){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	rtype ret = realcall(a1); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_2(rtype, call, realcall, arg_type1, arg_type2) \
rtype call(arg_type1 a1, arg_type2 a2){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	rtype ret = realcall(a1, a2); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_3(rtype, call, realcall, arg_type1, arg_type2, arg_type3) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	rtype ret = realcall(a1, a2, a3); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_4(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	rtype ret = realcall(a1, a2, a3, a4); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_5(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	rtype ret = realcall(a1, a2, a3, a4, a5); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_6(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5, arg_type6) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5, arg_type6 a6){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	_mth_ivk.push(a6); \
	rtype ret = realcall(a1, a2, a3, a4, a5, a6); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_7(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5, arg_type6, arg_type7) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5, arg_type6 a6, arg_type7 a7){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	_mth_ivk.push(a6); \
	_mth_ivk.push(a7); \
	rtype ret = realcall(a1, a2, a3, a4, a5, a6, a7); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_8(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5, arg_type6, arg_type7, arg_type8) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5, arg_type6 a6, arg_type7 a7, arg_type8 a8){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	_mth_ivk.push(a6); \
	_mth_ivk.push(a7); \
	_mth_ivk.push(a8); \
	rtype ret = realcall(a1, a2, a3, a4, a5, a6, a7, a8); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_9(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5, arg_type6, arg_type7, arg_type8, arg_type9) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5, arg_type6 a6, arg_type7 a7, arg_type8 a8, arg_type9 a9){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	_mth_ivk.push(a6); \
	_mth_ivk.push(a7); \
	_mth_ivk.push(a8); \
	_mth_ivk.push(a9); \
	rtype ret = realcall(a1, a2, a3, a4, a5, a6, a7, a8, a9); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

#define INTERFACE_TESTABLE_FUNCTION_10(rtype, call, realcall, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5, arg_type6, arg_type7, arg_type8, arg_type9, arg_type10) \
rtype call(arg_type1 a1, arg_type2 a2, arg_type3 a3, arg_type4 a4, arg_type5 a5, arg_type6 a6, arg_type7 a7, arg_type8 a8, arg_type9 a9, arg_type10 a10){ \
	method_invoke _mth_ivk(#call); \
	_mth_ivk.push(a1); \
	_mth_ivk.push(a2); \
	_mth_ivk.push(a3); \
	_mth_ivk.push(a4); \
	_mth_ivk.push(a5); \
	_mth_ivk.push(a6); \
	_mth_ivk.push(a7); \
	_mth_ivk.push(a8); \
	_mth_ivk.push(a9); \
	_mth_ivk.push(a10); \
	rtype ret = realcall(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); \
	_mth_ivk.set_return(ret); \
	interface_testor::push(_mth_ivk); \
	return ret; \
}

inline void non_call(...){
}

template<typename T>
T r_non_call(...){
	return T();
}

template<typename T>
T r_non_call_value(...){
	int i;
	return (T)(((long)&i) * (long)(time(NULL)));
}

int do_all_test(const char *name);

class test_util_allocator{
private:
	size_t _allocated;
public:
	test_util_allocator(): _allocated(0){}
	
	void* allocate FUNCTION_PARAMETER_LIST(size_t);
	char* allocate FUNCTION_PARAMETER_LIST(const char*, ...);
	void deallocate FUNCTION_PARAMETER_LIST(void*);
	static test_util_allocator& instance(){
		static test_util_allocator res;
		return res;
	}
	
	static void reset(){
		instance()._allocated = 0;
	}
	
	static size_t allocated(){
		return instance()._allocated;
	}
};

#endif //CCTEST_UTIL_H_
