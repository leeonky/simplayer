#include "ccmedia/video_player.h"

using CCLIBC::thread;
using CCLIBC::semaphore;
using CCLIBC::exception;
using CCSDL2::sdl_window;
using CCSDL2::sdl_renderer;
using CCSDL2::sdl_texture;
using CCSDL2::sdl_locked_txture;
using CCFFMPEG::soft_scaler;
using CCFFMPEG::video_frame;
using CCAPPLIB::time_util;

CCMEDIA_BG

namespace{
	static uint8_t cy = CCSDL2::rgb_2_y(0,0,0);
	static uint8_t cu = CCSDL2::rgb_2_u(0,0,0);
	static uint8_t cv = CCSDL2::rgb_2_v(0,0,0);
}

video_player::video_screen::video_screen FUNCTION_PARAMETER_LIST(CCSDL2::sdl_renderer &rnd, size_t w, size_t h)
	:_texture(TRACE_PARAMS(rnd, CCSDL2::sdl_texture::yv12, CCSDL2::sdl_texture::streaming,
		w, h, CCSDL2::sdl_renderer::copy)), _lock(TRACE_PARAMS(_texture)){
	sdl_locked_txture &lt = locked_txture();
	size_t s1 = lt.line()*h;
	size_t s2 = lt.line()/2*(h/2);
	uint8_t *pd1 = static_cast<uint8_t*>(lt.data());
	uint8_t *pd2 = pd1 + s1;
	uint8_t *pd3 = pd2 + s2;
	memset(pd1, cy, s1);
	memset(pd2, cu, s2);
	memset(pd3, cv, s2);
	locked_txture().update_texture();
}

video_player::video_player FUNCTION_PARAMETER_LIST(int x, int y, size_t w, size_t h, 
	sdl_window::style s, size_t bc, size_t ls): _x(x), _y(y), _w(w), _h(h), _s(s), _layout(none), _frame_duration(30),
	_running(false), _syn_sem(TRACE_PARAMS(1)),
	_frm_mutex(TRACE_PARAMS()), _buf_mutex(TRACE_PARAMS()), _frm_cond(TRACE_PARAMS()), _buf_cond(TRACE_PARAMS()),
	_least_buffer(ls), _most_buffer(bc), _less(true), _more(false), 
	_start(0), _clk_mutex(TRACE_PARAMS()), _pause_start(0) ,_clock(NULL), _subtitle_mutex(TRACE_PARAMS()), _subtitle(NULL)
	{
	semaphore TRACE_CONSTRUCT(wait_start);
	_thd.reassign(STACK_NEW(_pool, thread, TRACE_PARAMS(*this, &video_player::play_main, wait_start)));
	wait_start.wait();
}

video_player::~video_player(){
	_running = false;
	_thd->join();
}

void video_player::play_main(semaphore &start){
	sdl_window wnd(TRACE_PARAMS("", _x, _y, _w, _h, _s));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	_w = wnd.width();
	_h = wnd.height();
	for(size_t i=0; i<_most_buffer; ++i){
		_buffer.add(STACK_NEW(_pool, video_screen, TRACE_PARAMS(rnder, _w, _h)));
	}
	_running = true;
	bool present = false;
	int pts;
	start.post();
	video_screen *srn = NULL;
	while(_running){
		if(!present){
			{
				GUARD_LOCK(_frm_mutex);
				if(_frames.first() ||
					(_frm_cond.timed_wait(_frm_mutex, 10*1000*1000) && _frames.first())){
					srn = _frames.remove_first();
					update_state();
				}
			}
			if(srn){
				pts = srn->presentation_time();
				int ms = pts - current_clock();
				if((present = (ms > -33 && ms <= 2000))){
					srn->locked_txture().update_texture();
					rnder.copy_texture(srn->locked_txture().texture());
				}else{
					fprintf(stderr, "Drop frame:%d,%d\n", (int)pts, (int)pts - ms);
				}
				GUARD_LOCK(_buf_mutex);
				_buffer.add(srn);
				srn = NULL;
				_buf_cond.signal();
			}
		}
		if(present){
			if(_syn_sem.timed_wait(10*1000*1000)){
				_syn_sem.post();
				present = false;
				int ms = pts - current_clock();
				if(ms>0 && ms<=2000){
					thread::sleep_nano(ms*1000*1000);
				}
				rnder.present();
			}
		}
	}
	destruct_single_list_elements(_frames);
	destruct_single_list_elements(_buffer);
}

void video_player::change_scaler FUNCTION_PARAMETER_LIST(size_t sw, size_t sh, AVPixelFormat sf){
	if(AV_PIX_FMT_YUV420P != sf
		|| (width() != sw && height() != sh)){
		if(sw * height() != width() * sh){
			float z1 = ((double)width())/sw;
			float z2 = ((double)height())/sh;
			if(z1 < z2){
				_layout = vertical;
				_ph = sh * z1;
				_h1 = (height() - _ph) / 2;
				_h2 = height() - _h1 - _ph;
				_pw = width();
				_w1 = _w2 = 0;
			}else{
				_layout = horizontal;
				_pw = sw * z2;
				_w1 = (width() - _pw)/2;
				_w2 = width() - _w1 - _pw;
				_ph = height();
				_h1 = _h2 = 0;
			}
		}else{
			_layout = full;
			_pw = width();
			_ph = height();
			_h1 = _h2 = _w1 = _w2 = 0;
		}
		_sslr.reassign(OBJECT_NEW(soft_scaler,
			TRACE_PARAMS(sw, sh, sf, _pw, _ph, AV_PIX_FMT_YUV420P)));
	}else{
		if(width() == sw && height() == sh){
			_layout = same;
			_pw = width();
			_ph = height();
			_h1 = _h2 = _w1 = _w2 = 0;
		}else if(width() == sw){
			_layout = vertical_same;
			_ph = sh;
			_h1 = (height()-_ph)/2;
			_h2 = height()-_ph-_h1;
			_pw = width();
			_w1 = _w2 = 0;
		}else{
			_layout = horizontal_same;
			_pw = sw;
			_w1 = (width()-_pw)/2;
			_w2 = width()-_pw-_w1;
			_ph = height();
			_h1 = _h2 = 0;
		}
	}
}

void video_player::synchronize_clock FUNCTION_PARAMETER_LIST(size_t ms){
	GUARD_LOCK(_clk_mutex);
	_start = time_util::current_millisecond() - ms;
	if(_pause_start){
		_pause_start = CCAPPLIB::time_util::current_millisecond();
	}
}

bool video_player::push_video_noblock FUNCTION_PARAMETER_LIST(const CCFFMPEG::video_frame &frm){
	video_screen *srn;
	{
		GUARD_LOCK(_buf_mutex);
		if(!(_buffer.first() ||
			(_buf_cond.timed_wait(_buf_mutex, 10*1000*1000) && _buffer.first()))){
			return false;
		}
		srn = _buffer.remove_first();
	}
	AVFrame &f = *static_cast<AVFrame*>(frm.raw_data());
	sdl_locked_txture &lt = srn->locked_txture();
	if(none !=_layout){
		uint8_t *ps1 = f.data[0];
		int ls1 = f.linesize[0];
		uint8_t *ps2 = f.data[1];
		int ls2 = f.linesize[1];
		uint8_t *ps3 = f.data[2];
		int ls3 = f.linesize[2];
		uint8_t *pd1 = static_cast<uint8_t*>(lt.data());
		uint8_t *pd3 = pd1 + lt.line()*height();
		int ld = lt.line()/2, w = _pw/2;
		uint8_t *pd2 = pd3 + ld*(height()/2);
		
		if(_h1){
			size_t s1 = lt.line()*_h1;
			size_t s2 = ld*((_h1+1)/2);
			memset(pd1, cy, s1);
			memset(pd2, cu, s2);
			memset(pd3, cv, s2);
			pd1+=s1;
			pd2+=s2;
			pd3+=s2;
		}
		if(_w1){
			pd1+=_w1;
			pd2+=_w1/2;
			pd3+=_w1/2;
		}
		size_t end_y = _ph+_h1;
		if(_layout >= same){
			for(size_t y=_h1; y<end_y; ++y){
				memcpy(pd1, ps1, _pw);
				pd1+=lt.line();
				ps1+=ls1;
				if(!(y&1)){
					memcpy(pd2, ps2, w);
					memcpy(pd3, ps3, w);
					pd2+=ld;
					pd3+=ld;
					ps2+=ls2;
					ps3+=ls3;
				}
			}
		}else{
			void* buf_des[] = {pd1, pd2, pd3};
			int lines_des[] = {static_cast<int>(lt.line()), ld, ld};
			_sslr->scale((const void* const*)f.data, f.linesize, buf_des, lines_des);
			pd1+=_ph*lt.line();
			pd2+=(_ph+(_h1&1 ? 0:1))/2*ld;
			pd3+=(_ph+(_h1&1 ? 0:1))/2*ld;
		}
		if(_h2){
			size_t s1 = lt.line()*_h2;
			size_t s2 = ld*((_h2+(end_y&1 ? 0:1))/2);
			memset(pd1, cy, s1);
			memset(pd2, cu, s2);
			memset(pd3, cv, s2);
		}
	}
	{
		GUARD_LOCK(_subtitle_mutex);
		if(_subtitle){
			_subtitle->render_frame(lt, frm.presentation_time());
		}
	}
	srn->presentation_time(frm.presentation_time());
	{
		GUARD_LOCK(_clk_mutex);
		if(!_start){
			_start = time_util::current_millisecond() - frm.presentation_time() + _frame_duration;
			if(_pause_start){
				_pause_start = CCAPPLIB::time_util::current_millisecond();
			}
		}
	}
	{
		GUARD_LOCK(_frm_mutex);
		_frames.add(srn);
	}
	update_state();
	_frm_cond.signal();
	return true;
}

void video_player::push_video FUNCTION_PARAMETER_LIST(const CCFFMPEG::video_frame &frm){
	while(!push_video_noblock(frm));
}

void video_player::drop_all FUNCTION_PARAMETER_LIST(){
	GUARD_LOCK(_frm_mutex);
	GUARD_LOCK(_buf_mutex);
	while(_frames.size()){
		_buffer.add(_frames.remove_first());
	}
	update_state();
	GUARD_LOCK(_clk_mutex);
	_start = 0;
}

CCMEDIA_END
