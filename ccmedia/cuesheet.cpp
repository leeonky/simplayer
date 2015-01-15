#include "ccmedia/cuesheet.h"
#include "cclibc/io/file.h"
#include "cclibc/exception.h"
#include "ccapplib/string_util.h"

using CCAPPLIB::string_array;
using CCAPPLIB::string_util;
using CCLIBC::file;
using CCLIBC::allocator;
using CCLIBC::pool_ptr;
using CCLIBC::stack_pool;
using CCLIBC::exception;

namespace{
	bool load_value_if(char *line, const char *tag, stack_pool &pool, const char **item){
		char *p = strstr(line, tag);
		if(p && (p = index(p, '"'))){
			char *str = pool.allocate("%s", p+1);
			if((p = rindex(str, '"'))){
				*p = '\0';
			}
			*item = str;
			return true;
		}
		return false;
	}
}

CCMEDIA_BG

cuesheet::cuesheet FUNCTION_PARAMETER_LIST(const char *f):
	_performer(""), _title(""), _file(NULL), _full_path(NULL) {
	size_t l;
	pool_ptr<allocator> content(file::read_all_text<allocator>(TRACE_PARAMS(f, l)));
	string_array<> lines;
	string_util::split_line(static_cast<char*>(content.raw_pointer()), lines);
	for(size_t line = 0; line<lines.size() && !strstr(lines[line], "TRACK"); ++line){
		load_value_if(lines[line], "PERFORMER", _pool, &_performer)
			|| load_value_if(lines[line], "TITLE", _pool, &_title)
			|| load_value_if(lines[line], "FILE", _pool, &_file);
	}
	if(!_file || !strcmp(_file, "")){
		THROW_NEW_EXCEPTION(exception, "No 'FILE' in cue file: %s.", f);
	}
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "%s", f);
	char *p = rindex(buffer, '/');
	*(p ? p+1 : buffer) = '\0';
	_full_path = _pool.allocate("%s%s", buffer, _file);

	track *ct = NULL, *pct = NULL;
	for(size_t line = 0; line<lines.size(); ++line){
		if(strstr(lines[line], "TRACK")){
			pct = ct;
			_tracks.add(ct = STACK_NEW(_pool, track));
			int id = 0;
			if(1 == sscanf(lines[line], "%*s %d", &id)){
				ct->_index = id;
			}
		}else if(ct && strstr(lines[line], "INDEX 00")){
			int m,s,f;
			if(3 == sscanf(lines[line], "%*s %*s %d:%d:%d", &m, &s, &f)){
				ct->_start = (m*60+s)*1000 + f*1000/75;
				if(pct){
					pct->_end = ct->_start;
				}
			}
		}else if(ct && strstr(lines[line], "INDEX 01")){
			int m,s,f;
			if(3 == sscanf(lines[line], "%*s %*s %d:%d:%d", &m, &s, &f)){
				ct->_audio_start = (m*60+s)*1000 + f*1000/75;
				if(!ct->_start){
					ct->_start = ct->_audio_start;
					if(pct){
						pct->_end = ct->_start;
					}
				}
			}
		}else if(ct){
			load_value_if(lines[line], "PERFORMER", _pool, &ct->_performer)
				|| load_value_if(lines[line], "TITLE", _pool, &ct->_title);
		}
	}
}

CCMEDIA_END
