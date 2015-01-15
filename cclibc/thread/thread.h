#ifndef CCLIBC_THREAD_THREAD_H_
#define CCLIBC_THREAD_THREAD_H_

#include <pthread.h>
#include <memory>
#include "cclibc/env.h"
#include "cclibc/assert.h"
#include "cclibc/callc.h"
#include "cclibc/trace.h"
#include "cclibc/memory.h"

CCLIBC_BG

#ifdef STACK_TRACE_FLAG

#define stack_size(...) stack_size(INVOKE_TRACE, ##__VA_ARGS__) 
#define detached(...) detached(INVOKE_TRACE, ##__VA_ARGS__) 
#define sleep_nano(...) sleep_nano(INVOKE_TRACE, ## __VA_ARGS__) 
#define exit(...) exit(INVOKE_TRACE, ##__VA_ARGS__) 
#define join(...) join(INVOKE_TRACE, ##__VA_ARGS__) 
#define cancel(...) cancel(INVOKE_TRACE, ##__VA_ARGS__) 

#endif

class thread;
class thread_attribute {
friend class thread;
private:
	pthread_attr_t _attr;

public:
	thread_attribute FUNCTION_PARAMETER_LIST(){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_attr_init, &_attr);
	}

	~thread_attribute(){
		debug_assert(!pthread_attr_destroy(&_attr));
	}

	size_t stack_size FUNCTION_PARAMETER_LIST() const {
		size_t r;
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_attr_getstacksize, &_attr, &r);
		return r;
	}

	thread_attribute& stack_size FUNCTION_PARAMETER_LIST(size_t s){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_attr_setstacksize, &_attr, s);
		return *this;
	}

	bool detached FUNCTION_PARAMETER_LIST() const {
		int r;
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_attr_getdetachstate, &_attr, &r);
		return PTHREAD_CREATE_DETACHED == r;
	}

	thread_attribute& detached FUNCTION_PARAMETER_LIST(bool b) {
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_attr_setdetachstate, &_attr, 
			static_cast<int>(b ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE));
		return *this;
	}
};

struct run_only{
public:
	thread &_thd;
	void (*_run)();
	run_only(thread &thd, void(*run)()): _thd(thd), _run(run){}
	static void* thread_run_only(void*);
};

template<typename P, typename A>
struct run_arg{
public:
	thread &_thd;
	void (*_run)(P);
	A _arg;
	run_arg(thread &thd, void(*run)(P), A arg): _thd(thd), _run(run), _arg(arg){}
	static void* thread_run_arg(void*);
};

template<typename I, typename F>
struct run_type{
public:
	thread &_thd;
	I _ins;
	F _run;
	run_type(thread &thd, I ins, F run): _thd(thd), _ins(ins), _run(run){}
	static void* thread_run_type(void*);
};

template<typename I, typename A, typename F>
struct run_type_arg{
public:
	thread &_thd;
	I _ins;
	F _run;
	A _arg;
	run_type_arg(thread &thd, I ins, F run, A arg): _thd(thd), _ins(ins), _run(run), _arg(arg){}
	static void* thread_run_type_arg(void*);
};

class thread {
friend struct run_only;
template<typename P, typename A> friend struct run_arg;
template<typename I, typename F> friend struct run_type;
template<typename I, typename A, typename F> friend struct run_type_arg;
private:
	pthread_t _tid;
#ifdef STACK_TRACE_FLAG
	stack *_running_stack, *_exp_stack;
#endif
	stack_pool _thread_pool;
	stack_pool _exception_pool;
	allocator _allocator;
	
	//should call this in main thread because new method will stack trace.
	void init_env(){
#ifdef STACK_TRACE_FLAG
		stack_ptr<stack> rs(STACK_NEW(_thread_pool, stack));
		stack_ptr<stack> es(STACK_NEW(_thread_pool, stack));
		_running_stack = rs.release();
		_exp_stack = es.release();
#endif
	}
	
	void init_env_thread(){
		stack_pool::set_thread_pool(&_thread_pool);
		exception::set_exception_pool(&_exception_pool);
		allocator::set_instance(&_allocator);
#ifdef STACK_TRACE_FLAG
		stack::set_running_stack(_running_stack);
		exception::set_last_stack(_exp_stack);
#endif
	}
	
	void init(thread &thd, void (*run)(), const pthread_attr_t *attr){
		run_only *p = STACK_NEW(_thread_pool, run_only, thd, run);
		try{
			init_env();
			INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_create, &_tid, attr, p->thread_run_only, p);
		}catch(...){
			p->~run_only();
			p = NULL;
			throw;
		}
	}

	template<typename P, typename A>
	void init_arg(thread &thd, void (*run) (P), A arg, const pthread_attr_t *attr){
		typedef run_arg<P, A> run_arg_p_a;
		run_arg_p_a *p = STACK_NEW(_thread_pool, run_arg_p_a, thd, run, arg);
		try{
			init_env();
			INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_create, &_tid, attr, p->thread_run_arg, p);
		}catch(...){
			p->~run_arg_p_a();
			p = NULL;
			throw;
		}
	}

	template<typename I, typename F>
	void init_type(thread &thd, I ins, F run, const pthread_attr_t *attr){
		typedef run_type<I,F> run_type_i_f;
		run_type_i_f *p = STACK_NEW(_thread_pool, run_type_i_f, thd, ins, run);
		try{
			init_env();
			INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_create, &_tid, attr, p->thread_run_type, p);
		}catch(...){
			p->~run_type_i_f();
			p = NULL;
			throw;
		}
	}

	template<typename I, typename A, typename F>
	void init_type_arg(thread &thd, I ins, F run, A arg, const pthread_attr_t *attr){
		typedef run_type_arg<I,A,F> run_type_arg_i_a_f;
		run_type_arg_i_a_f *p = STACK_NEW(_thread_pool, run_type_arg_i_a_f, thd, ins, run, arg);
		try{
			init_env();
			INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_create, &_tid, attr, p->thread_run_type_arg, p);
		}catch(...){
			p->~run_type_arg_i_a_f();
			p = NULL;
			throw;
		}
	}
	thread(const thread&);
	thread& operator=(const thread&);

public:
	thread FUNCTION_PARAMETER_LIST(void (*run)(), const thread_attribute &attr){
		init(*this, run, &attr._attr);
	};

	thread FUNCTION_PARAMETER_LIST(void (*run)()){
		init(*this, run, NULL);
	}

	template<typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(void (*run)(P), A& arg, const thread_attribute &attr){
		init_arg<P, A&>(*this, run, arg, &attr._attr);
	}

	template<typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(void (*run)(P), A& arg){
		init_arg<P, A&>(*this, run, arg, NULL);
	}

	template<typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(void (*run)(P), const A& arg, const thread_attribute &attr){
		init_arg<P, const A&>(*this, run, arg, &attr._attr);
	}

	template<typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(void (*run)(P), const A& arg){
		init_arg<P, const A&>(*this, run, arg, NULL);
	}

	template<typename T>
	thread FUNCTION_PARAMETER_LIST(T &ins, void (T::*run)(), const thread_attribute &attr){
		init_type<T&>(*this, ins, run, &attr._attr);
	}

	template<typename T>
	thread FUNCTION_PARAMETER_LIST(T &ins, void (T::*run)()){
		init_type<T&>(*this, ins, run, NULL);
	}
	
	template<typename T>
	thread FUNCTION_PARAMETER_LIST(const T &ins, void (T::*run)() const, const thread_attribute &attr){
		init_type<const T&>(*this, ins, run, &attr._attr);
	}

	template<typename T>
	thread FUNCTION_PARAMETER_LIST(const T &ins, void (T::*run)() const){
		init_type<const T&>(*this, ins, run, NULL);
	}

	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(T &ins, void (T::*run)(P), A& arg, const thread_attribute &attr){
		init_type_arg<T&,A&>(*this, ins, run, arg, &attr._attr);
	}

	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(T &ins, void (T::*run)(P), A& arg){
		init_type_arg<T&,A&>(*this, ins, run, arg, NULL);
	}
	
	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(T &ins, void (T::*run)(P), const A& arg, const thread_attribute &attr){
		init_type_arg<T&,const A&>(*this, ins, run, arg, &attr._attr);
	}

	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(T &ins, void (T::*run)(P), const A& arg){
		init_type_arg<T&,const A&>(*this, ins, run, arg, NULL);
	}

	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(const T &ins, void (T::*run)(P) const, A& arg, const thread_attribute &attr){
		init_type_arg<const T&,A&>(*this, ins, run, arg, &attr._attr);
	}

	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(const T &ins, void (T::*run)(P) const, A& arg){
		init_type_arg<const T&,A&>(*this, ins, run, arg, NULL);
	}
	
	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(const T &ins, void (T::*run)(P) const, const A& arg, const thread_attribute &attr){
		init_type_arg<const T&,const A&>(*this, ins, run, arg, &attr._attr);
	}

	template<typename T, typename P, typename A>
	thread FUNCTION_PARAMETER_LIST(const T &ins, void (T::*run)(P) const, const A& arg){
		init_type_arg<const T&,const A&>(*this, ins, run, arg, NULL);
	}

	void join FUNCTION_PARAMETER_LIST(){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_join, _tid, NULL);
	}

	void cancel FUNCTION_PARAMETER_LIST(){
		INVOKE_THROW_RETURN(int, != 0, posix_error, pthread_cancel, _tid); 
	}

	static void exit FUNCTION_PARAMETER_LIST(){
		pthread_exit(NULL);
	}

	static bool sleep_nano FUNCTION_PARAMETER_LIST(uint64_t &ns);
	static void sleep_nano FUNCTION_PARAMETER_LIST(const uint64_t &ns);
};

inline void* run_only::thread_run_only(void *arg){
	buffer_pool_list _pool_list;
	buffer_pool::set_pool_list(&_pool_list);
	stack_ptr<run_only> a(static_cast<run_only*>(arg));
	a->_thd.init_env_thread();
	if(a->_run){
		try{
			a->_run();
		}catch(const exception *e){
			fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
#ifdef STACK_TRACE_FLAG
			exception::last_stack().print_stack(stderr);
#endif
		}
	}
	return NULL;
}

template<typename P, typename A>
inline void* run_arg<P, A>::thread_run_arg(void *arg){
	buffer_pool_list _pool_list;
	buffer_pool::set_pool_list(&_pool_list);
	stack_ptr<run_arg<P, A> > a(static_cast<run_arg<P, A>*>(arg));
	a->_thd.init_env_thread();
	if(a->_run){
		try{
			a->_run(a->_arg);
		}catch(const exception *e){
			fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
#ifdef STACK_TRACE_FLAG
			exception::last_stack().print_stack(stderr);
#endif
		}
	}
	return NULL;
}

template<typename I, typename F>
inline void* run_type<I,F>::thread_run_type(void *arg) {
	buffer_pool_list _pool_list;
	buffer_pool::set_pool_list(&_pool_list);
	stack_ptr<run_type<I,F> > a(static_cast<run_type<I,F>*>(arg));
	a->_thd.init_env_thread();
	if(a->_run){
		try{
			(a->_ins.*a->_run)();
		}catch(const exception *e){
			fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
#ifdef STACK_TRACE_FLAG
			exception::last_stack().print_stack(stderr);
#endif
		}
	}
	return NULL;
}

template<typename I, typename A, typename F>
inline void* run_type_arg<I,A,F>::thread_run_type_arg(void *arg) {
	buffer_pool_list _pool_list;
	buffer_pool::set_pool_list(&_pool_list);
	stack_ptr<run_type_arg<I,A,F> > a(static_cast<run_type_arg<I,A,F>*>(arg));
	a->_thd.init_env_thread();
	if(a->_run){
		try{
			(a->_ins.*a->_run)(a->_arg);
		}catch(const exception *e){
			fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
#ifdef STACK_TRACE_FLAG
			exception::last_stack().print_stack(stderr);
#endif
		}
	}
	return NULL;
}

CCLIBC_END

#endif //CCLIBC_THREAD_THREAD_H_
