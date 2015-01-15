#include "ccapplib/command_list.h"
#include "ccapplib/string_util.h"
#include "cclibc/io/file.h"
#include "cclibc/util/array_list.h"

using CCLIBC::file;
using CCLIBC::pool_ptr;
using CCLIBC::allocator;
using CCLIBC::array_list;

CCAPPLIB_BG

command_list::command_list FUNCTION_PARAMETER_LIST(const char *arg0, const char *file){
	size_t fl;
	pool_ptr<allocator> content(file::read_all_text<allocator>(TRACE_PARAMS(file, fl)));
	string_array<> lines;
	string_util::split_line(static_cast<char*>(content.raw_pointer()), lines);
	for(size_t line=0; line<lines.size(); ++line){
		if(strcmp("", lines[line])){
			array_list<char*> args;
			args.add(_pool.allocate("%s", arg0));
			for(; line<lines.size() && strcmp("", lines[line]); ++line){
				args.add(_pool.allocate("%s", lines[line]));
			}
			size_t len = args.size() * sizeof(char*);
			char** arg_array = static_cast<char**>(_pool.allocate(len));
			memcpy(arg_array, &args[0], len);
			_parameters.add(STACK_NEW(_pool, command_parameter, args.size(), arg_array));
		}
	}
}

CCAPPLIB_END
