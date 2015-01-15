#include "ccmedia_mock_test/video_player_test.h"
#include "ccmedia/video_player.h"
#include "cclibc/thread/thread.h"
#include "ccapplib/time_util.h"

using CCMEDIA::video_player;
using CCMEDIA::clock_source;
using CCLIBC::thread;
using CCLIBC::thread_attribute;
using CCLIBC::mutex;
using CCSDL2::sdl_window;
using CCFFMPEG::video_frame;

void video_player_test_class::video_player_allocate_screen_buffer(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 300, sdl_window::none, 30));
	CPPUNIT_ASSERT_EQUAL((size_t)30, vpl._buffer.size());
}

void video_player_test_class::video_player_push_picture(){
	video_frame TRACE_CONSTRUCT(vfm);
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 1));
	vpl.frame_duration(30);
	vfm.presentation_time(200);
	vpl.push_video(vfm);
	vfm.presentation_time(400);
	vpl.push_video(vfm);
	CPPUNIT_ASSERT_EQUAL((size_t)1, vpl._frames.size());
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._buffer.size());
}

void video_player_test_class::video_player_consume(){
	video_frame TRACE_CONSTRUCT(vfm);
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 1));
	vfm.presentation_time(200);
	vpl.push_video(vfm);
	vfm.presentation_time(400);
	vpl.push_video(vfm);
	thread::sleep_nano(400*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._frames.size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, vpl._buffer.size());
}

void video_player_test_class::video_player_drain(){
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(100);
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 5));
	for(int i=0; i<5 ;i++){
		vfm.presentation_time(100+vfm.presentation_time());
		vpl.push_video(vfm);
	}
	vpl.drop_all();
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._frames.size());
}

void video_player_test_class::video_player_clock(){
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 1));
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(30);
	vpl.push_video(vfm);
	thread::sleep_nano(200*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)2, vpl.current_clock()/100);
}

void video_player_test_class::video_player_drop_frame(){
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(-40);
	CCAPPLIB::clock clk;
	{
		video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 1));
		clk.last_millisecond();
		vpl.push_video(vfm);
		thread::sleep_nano(1*1000*1000);
	}
	CPPUNIT_ASSERT(clk.last_millisecond() <= 45);
}

void video_player_test_class::video_player_pause(){
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(50);
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 2));
	vpl.pause();
	vpl.push_video(vfm);
	vfm.presentation_time(80);
	vpl.push_video(vfm);
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)1, vpl._frames.size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, vpl._buffer.size());
}

void video_player_test_class::video_player_resume(){
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(150);
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 2));
	vpl.pause();
	vpl.push_video(vfm);
	vfm.presentation_time(200);
	vpl.push_video(vfm);
	thread::sleep_nano(100*1000*1000);
	vpl.resume();
	thread::sleep_nano(200*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)2, vpl._buffer.size());
}

void video_player_test_class::video_player_pause_time(){
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 2));
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(30);
	vpl.push_video(vfm);
	vpl.pause();
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl.current_clock()/5);
}

void video_player_test_class::video_player_pause_resume_time(){
	video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 2));
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(30);
	vpl.push_video(vfm);
	vpl.pause();
	thread::sleep_nano(100*1000*1000);
	vpl.resume();
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)1, (vpl.current_clock()+10)/100);
}

namespace{
	class slow_clk: public clock_source{
	public:
		size_t current_clock FUNCTION_PARAMETER_LIST(){
			static CCAPPLIB::clock clk;
			return clk.last_millisecond() - 100;
		}
	};
}

void video_player_test_class::video_player_change_clock(){
	video_frame TRACE_CONSTRUCT(vfm);
	vfm.presentation_time(0);
	slow_clk sclk;
	CCAPPLIB::clock clk;
	{
		video_player vpl(TRACE_PARAMS(0, 0, 960, 1200, sdl_window::none, 1));
		vpl.change_clock(&sclk);
		vpl.push_video(vfm);
		thread::sleep_nano(2*1000*1000);
	}
	CPPUNIT_ASSERT_EQUAL((size_t)1, (clk.last_millisecond()+50)/100);
}
