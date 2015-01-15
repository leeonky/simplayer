#ifndef CCAPPLIB_COMMAND_LIST_H_
#define CCAPPLIB_COMMAND_LIST_H_

#include "ccapplib/env.h"
#include "cclibc/trace.h"
#include "cclibc/memory.h"
#include "cclibc/util/list.h"

CCAPPLIB_BG

class command_list{
public:
	class command_parameter: public CCLIBC::dual_list<command_parameter>::node{
	private:
		int _count;
		char ** _arguments;
	public:
		command_parameter(int c, char **args): _count(c), _arguments(args) {}
		int count() const{
			return _count;
		}
		char** arguments() const{
			return _arguments;
		}
	};
	command_list FUNCTION_PARAMETER_LIST(const char*, const char*);
	const CCLIBC::dual_list<command_parameter>& parameters() const{
		return _parameters;
	}
private:
	CCLIBC::stack_pool _pool;
	CCLIBC::dual_list<command_parameter> _parameters;
};

CCAPPLIB_END

#endif //CCAPPLIB_COMMAND_LIST_H_
