#ifndef CCMEDIA_VIDEO_PLAYER_H_
#define CCMEDIA_VIDEO_PLAYER_H_

#include "ccmedia/env.h"
#include "ccmedia/util.h"
#include "ccmedia/subtitle_decoder.h"
#include "cclibc/trace.h"
#include "cclibc/memory.h"
#include "cclibc/thread/thread.h"
#include "cclibc/thread/semaphore.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/thread/condition.h"
#include "ccsdl2/video.h"
#include "ccffmpeg/ffmpeg.h"
#include "ccapplib/time_util.h"

CCMEDIA_BG

#ifdef STACK_TRACE_FLAG

#define push_video(...) push_video(INVOKE_TRACE, ##__VA_ARGS__)
#define push_video_noblock(...) push_video_noblock(INVOKE_TRACE, ##__VA_ARGS__)
#define change_scaler(...) change_scaler(INVOKE_TRACE, ##__VA_ARGS__)
#ifndef drop_all
#define drop_all(...) drop_all(INVOKE_TRACE, ##__VA_ARGS__)
#endif 
#define change_clock(...) change_clock(INVOKE_TRACE, ##__VA_ARGS__)
#define synchronize_clock(...) synchronize_clock(INVOKE_TRACE, ##__VA_ARGS__)

#endif

class video_player{
private:
	int _x,_y;
	size_t _w, _h;
	CCSDL2::sdl_window::style _s;
	CCLIBC::object_ptr<CCFFMPEG::soft_scaler> _sslr;
	size_t _w1, _h1, _pw, _ph, _w2, _h2;
	enum layout{
		none, full, vertical, horizontal,
		same, vertical_same, horizontal_same,
	} _layout;

	size_t _frame_duration;

	bool _running;
	CCLIBC::stack_pool _pool;
	CCLIBC::stack_ptr<CCLIBC::thread> _thd;
	CCLIBC::semaphore _syn_sem;

	class video_screen: public CCLIBC::single_list<video_screen>::node{
	private:
		CCSDL2::sdl_texture _texture;
		CCSDL2::sdl_locked_txture _lock;
		size_t _pts;
	public:
		video_screen FUNCTION_PARAMETER_LIST(CCSDL2::sdl_renderer &rnd, size_t w, size_t h);
		CCSDL2::sdl_locked_txture& locked_txture() {
			return _lock;
		}
		size_t presentation_time(){
			return _pts;
		}
		void presentation_time(size_t t){
			_pts = t;
		}
	};

	CCLIBC::single_list<video_screen> _frames, _buffer;
	CCLIBC::mutex _frm_mutex, _buf_mutex;
	CCLIBC::condition _frm_cond, _buf_cond;

	const size_t _least_buffer, _most_buffer;
	bool _less, _more;
	void update_state(){
		_less = _frames.size() < _least_buffer;
		_more = _frames.size() >= _most_buffer;
	}
	
	uint64_t _start;
	CCLIBC::mutex _clk_mutex;
	uint64_t _pause_start;
	clock_source *_clock;
	CCLIBC::mutex _subtitle_mutex;
	subtitle_decoder *_subtitle;

	void play_main(CCLIBC::semaphore&);
	
public:
	video_player FUNCTION_PARAMETER_LIST(int, int, size_t, size_t,
		CCSDL2::sdl_window::style s = CCSDL2::sdl_window::none, size_t bc = 24, size_t ls = 5);
	~video_player();

	int start_x() const{
		return _x;
	}

	int start_y() const{
		return _y;
	}

	size_t width() const{
		return _w;
	}

	size_t height() const{
		return _h;
	}

	size_t picture_width() const{
		return _pw;
	}

	size_t picture_height() const{
		return _ph;
	}
	
	size_t frame_duration() const{
		return _frame_duration;
	}

	void frame_duration(size_t ms){
		_frame_duration = ms;
	}

	void change_scaler FUNCTION_PARAMETER_LIST(size_t, size_t, AVPixelFormat);

	void push_video FUNCTION_PARAMETER_LIST(const CCFFMPEG::video_frame&);
	bool push_video_noblock FUNCTION_PARAMETER_LIST(const CCFFMPEG::video_frame&);
	void drop_all FUNCTION_PARAMETER_LIST();

	void pause() {
		_syn_sem.wait();
		GUARD_LOCK(_clk_mutex);
		_pause_start = CCAPPLIB::time_util::current_millisecond();
	}

	void resume() {
		GUARD_LOCK(_clk_mutex);
		if(_pause_start){
			_start += (CCAPPLIB::time_util::current_millisecond()-_pause_start);
			_pause_start = 0;
		}
		_syn_sem.post();
	}

	size_t current_clock FUNCTION_PARAMETER_LIST(){
		GUARD_LOCK(_clk_mutex);
		if(_clock){
			return _clock->current_clock();
		}
		if(_start){
			if(_pause_start){
				return _pause_start - _start;
			}
			return CCAPPLIB::time_util::current_millisecond() - _start;
		}else{
			return 0;
		}
	}

	void change_clock FUNCTION_PARAMETER_LIST(clock_source *clk){
		GUARD_LOCK(_clk_mutex);
		_clock = clk;
	}
	void synchronize_clock FUNCTION_PARAMETER_LIST(size_t);
	
	void change_subtitle(subtitle_decoder *sub){
		GUARD_LOCK(_subtitle_mutex);
		_subtitle = sub;
	}

	bool insufficient() const{
		return _less;
	}

	bool overflow() const{
		return _more;
	}
};

CCMEDIA_END

#endif //CCMEDIA_VIDEO_PLAYER_H_

