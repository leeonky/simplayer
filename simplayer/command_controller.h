#ifndef SIMPLAYER_COMMAND_CONTROLLER_H_
#define SIMPLAYER_COMMAND_CONTROLLER_H_

#include "simplayer/env.h"
#include "cclibc/util/map.h"
#include "cclibc/memory.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/thread/thread.h"

SIMPLAYER_BG

template<typename T>
class command_controller{
private:
	char buf[1024];
	const char* char_map[256];
	bool _running;
	class command_handler{
	public:
		bool(*_fun)(const char*, T&);
		T &_arg;
		command_handler(bool(*fun)(const char*, T&), T &arg): _fun(fun), _arg(arg){}
	};
	CCLIBC::map<const char*, command_handler*> handlers;
	CCLIBC::stack_pool pool;
	CCLIBC::mutex _mutex;
	CCLIBC::thread _thd;
	void thread_main(){
		char tmp[sizeof(buf)];
		for(;;){
			fgets(tmp, sizeof(tmp), stdin);
			send_command(tmp);
		}
	}
public:
	command_controller(): _running(true), _mutex(TRACE_PARAMS()), _thd(TRACE_PARAMS(*this, &command_controller::thread_main)){
		*buf = '\0';
		bzero(char_map, sizeof(char_map));
	}
	~command_controller(){
		_thd.cancel();
		_thd.join();
	}
	bool running() const{
		return _running;
	}
	void regist_command(const char *cmd, bool(*fun)(const char*, T&), T &arg){
		GUARD_LOCK(_mutex);
		const char *cmd_word = pool.allocate(cmd);
		handlers.add(cmd_word, STACK_NEW(pool, command_handler, fun, arg));
		char_map[static_cast<uint8_t>(cmd_word[0])] = cmd_word;
	}
	void clear_handler(){
		handlers.clear();
		pool.reset();
		bzero(char_map, sizeof(char_map));
	}
	void send_command(const char *cmd){
		GUARD_LOCK(_mutex);
		strncpy(buf, cmd, sizeof(buf));
	}
	void command_processing(){
		GUARD_LOCK(_mutex);
		if(*buf){
			char cmd[128];
			if(1 == sscanf(buf, "%s", cmd)){
				typename CCLIBC::map<const char*, command_handler*>::entry *e = handlers.get_entry(cmd);
				if(!e && char_map[static_cast<uint8_t>(cmd[0])]){
					e = handlers.get_entry(char_map[static_cast<uint8_t>(cmd[0])]);
				}
				if(e){
					_running = e->value()->_fun(buf, e->value()->_arg);
				}
			}
			*buf = '\0';
		}
	}
};

SIMPLAYER_END

#endif //SIMPLAYER_COMMAND_CONTROLLER_H_
