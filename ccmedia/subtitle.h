#ifndef CCMEDIA_SUBTILTL_H
#define CCMEDIA_SUBTILTL_H

#include "ccmedia/env.h"
#include "ccapplib/time_util.h"
#include "ccsdl2/video.h"

CCMEDIA_BG

class subtitle_stream{
public:
	virtual ~subtitle_stream() {};
	virtual void render_frame FUNCTION_PARAMETER_LIST(CCSDL2::sdl_locked_txture&, size_t) = 0;
	virtual void extend_content FUNCTION_PARAMETER_LIST(const char*, const CCAPPLIB::time_bucket&) = 0;
	virtual void clear_cache() = 0;
};

#ifdef STACK_TRACE_FLAG

#define render_frame(...) render_frame(INVOKE_TRACE, ##__VA_ARGS__)
#define extend_content(...) extend_content(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCMEDIA_END

#endif //CCMEDIA_SUBTILTL_H
