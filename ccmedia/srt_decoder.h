#ifndef CCMEDIA_SRT_DECORDER_H_
#define CCMEDIA_SRT_DECORDER_H_

#include "ccmedia/env.h"
#include "ccsdl2/ttf.h"
#include "cclibc/util/map.h"
#include "ccmedia/subtitle.h"

CCMEDIA_BG

class srt_stream;
class srt_decoder: public CCLIBC::object{
friend class srt_stream;
private:
	CCSDL2::rich_text_renderer _renderer;
	float _bottom;
public:
	CCSDL2::rich_text_renderer& renderer() {
		return _renderer;
	}
	srt_decoder FUNCTION_PARAMETER_LIST(const char*, size_t, CCSDL2::sdl_color, float _bottom = 0.95);
};

class subtitle_time_comparer{
public:
	int compare(const CCAPPLIB::time_bucket &t1, const CCAPPLIB::time_bucket &t2) const{
		if(t1.start() < t2.start()){
			return -1;
		}else if(t2.contains(t1.start())){
			return 0;
		}else{
			return 1;
		}
	}
};

class srt_stream: public subtitle_stream{
private:
	CCLIBC::stack_pool _pool;
	srt_decoder &_decoder;
	CCLIBC::map<CCAPPLIB::time_bucket, CCLIBC::array_list<const char*>, subtitle_time_comparer> _contents;
	typedef CCLIBC::map<CCAPPLIB::time_bucket, CCLIBC::array_list<const char*>, subtitle_time_comparer>::entry_type srt_event;
	size_t _max_len;
	srt_event *_last_event;
	CCLIBC::array_list<CCSDL2::sdl_surface> _last_imgs;
	void extend_content FUNCTION_PARAMETER_LIST(const char*);
public:
	srt_stream FUNCTION_PARAMETER_LIST(srt_decoder&, const char*, const char *cs = "UTF-8");
	srt_stream(srt_decoder&, char *s=const_cast<char*>(""), size_t l=0);
	void render_frame FUNCTION_PARAMETER_LIST(CCSDL2::sdl_locked_txture&, size_t);
	void extend_content FUNCTION_PARAMETER_LIST(const char*, const CCAPPLIB::time_bucket&);
	void clear_cache(){_last_imgs.clear();};
};

#ifdef STACK_TRACE_FLAG

#define font_size(...) font_size(INVOKE_TRACE, ##__VA_ARGS__)
#define extend_content(...) extend_content(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCMEDIA_END

#endif //CCMEDIA_SRT_DECORDER_H_

