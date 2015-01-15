#ifndef CCMEDIA_CUESHEET_H_
#define CCMEDIA_CUESHEET_H_

#include "ccmedia/env.h"
#include "cclibc/trace.h"
#include "cclibc/memory.h"
#include "cclibc/util/list.h"

CCMEDIA_BG

class cuesheet{
public:
	class track: public CCLIBC::dual_list<track>::node{
	friend class cuesheet;
	private:
		const char *_performer;
		const char *_title;
		size_t _index;
		size_t _start;
		size_t _audio_start;
		size_t _end;
	public:
		track(): _performer(""), _title(""), _start(0), _audio_start(0), _end(0) {}
		const char* performer() const{
			return _performer;
		}
		const char* title() const{
			return _title;
		}
		size_t index() const{
			return _index;
		}
		size_t start() const{
			return _start;
		}
		size_t audio_start() const{
			return _audio_start;
		}
		size_t end() const{
			return _end;
		}
	};
	cuesheet FUNCTION_PARAMETER_LIST(const char*);
	
	const char* performer() const{
		return _performer;
	}
	
	const char* title() const{
		return _title;
	}
	
	const char* file() const{
		return _file;
	}
	
	const char* full_path() const{
		return _full_path;
	}

	const CCLIBC::dual_list<track>& tracks() const{
		return _tracks;
	}
	
	const track* find_track(size_t ms) const{
		LIST_FOREACH(const track, tr, _tracks){
			if(tr->end()){
				if(tr->end() > ms && ms >= tr->start()){
					return tr;
				}
			}else if(ms >= tr->start()){
				return tr;
			}
		}
		return NULL;
	}

private:
	CCLIBC::stack_pool _pool;
	const char *_performer;
	const char *_title;
	const char *_file;
	const char *_full_path;
	CCLIBC::dual_list<track> _tracks;
};

CCMEDIA_END

#endif //CCMEDIA_CUESHEET_H_
