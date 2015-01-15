#include "ccmedia_test/video_player_test.h"
#include "ccmedia/video_player.h"
#include "ccffmpeg/ffmpeg.h"

using CCSDL2::sdl_window;
using CCSDL2::sdl_rect;
using CCFFMPEG::video_frame;
using CCFFMPEG::video_decoder;
using CCFFMPEG::media_stream;
using CCFFMPEG::media_packet;
using CCMEDIA::video_player;
using CCLIBC::thread;

void video_player_test_class::video_player_new_delete(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 225, sdl_window::none));
	CPPUNIT_ASSERT_EQUAL(vpl.start_x(), 0);
	CPPUNIT_ASSERT_EQUAL(vpl.start_y(), 0);
	CPPUNIT_ASSERT_EQUAL(vpl.width(), (size_t)400);
	CPPUNIT_ASSERT_EQUAL(vpl.height(), (size_t)225);
	CPPUNIT_ASSERT_EQUAL(vpl._s, sdl_window::none);
	CPPUNIT_ASSERT(vpl._running);
}

void video_player_test_class::video_player_set_scaler_match(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 300, sdl_window::none));
	vpl.change_scaler(40, 30, AV_PIX_FMT_RGB24);
	CPPUNIT_ASSERT_EQUAL(video_player::full, vpl._layout);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w2);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h2);
	CPPUNIT_ASSERT_EQUAL((size_t)400, vpl._pw);
	CPPUNIT_ASSERT_EQUAL((size_t)300, vpl._ph);
}

void video_player_test_class::video_player_set_scaler_16_9_to_4_3(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 300, sdl_window::none));
	vpl.change_scaler(16, 9, AV_PIX_FMT_RGB24);
	CPPUNIT_ASSERT_EQUAL(video_player::vertical, vpl._layout);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w2);
	CPPUNIT_ASSERT_EQUAL((size_t)37, vpl._h1);
	CPPUNIT_ASSERT_EQUAL((size_t)38, vpl._h2);
	CPPUNIT_ASSERT_EQUAL((size_t)400, vpl._pw);
	CPPUNIT_ASSERT_EQUAL((size_t)225, vpl._ph);
}

void video_player_test_class::video_player_set_scaler_4_3_to_16_9(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 225, sdl_window::none));
	vpl.change_scaler(4, 3, AV_PIX_FMT_RGB24);
	CPPUNIT_ASSERT_EQUAL(video_player::horizontal, vpl._layout);
	CPPUNIT_ASSERT_EQUAL((size_t)50, vpl._w1);
	CPPUNIT_ASSERT_EQUAL((size_t)50, vpl._w2);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h2);
	CPPUNIT_ASSERT_EQUAL((size_t)300, vpl._pw);
	CPPUNIT_ASSERT_EQUAL((size_t)225, vpl._ph);
}

void video_player_test_class::video_player_set_scaler_same(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 225, sdl_window::none));
	vpl.change_scaler(400, 225, AV_PIX_FMT_YUV420P);
	CPPUNIT_ASSERT_EQUAL(video_player::same, vpl._layout);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w2);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h2);
	CPPUNIT_ASSERT_EQUAL((size_t)400, vpl._pw);
	CPPUNIT_ASSERT_EQUAL((size_t)225, vpl._ph);
}

void video_player_test_class::video_player_set_scaler_v_same(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 225, sdl_window::none));
	vpl.change_scaler(400, 200, AV_PIX_FMT_YUV420P);
	CPPUNIT_ASSERT_EQUAL(video_player::vertical_same, vpl._layout);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._w2);
	CPPUNIT_ASSERT_EQUAL((size_t)12, vpl._h1);
	CPPUNIT_ASSERT_EQUAL((size_t)13, vpl._h2);
	CPPUNIT_ASSERT_EQUAL((size_t)400, vpl._pw);
	CPPUNIT_ASSERT_EQUAL((size_t)200, vpl._ph);
}

void video_player_test_class::video_player_set_scaler_h_same(){
	video_player vpl(TRACE_PARAMS(0, 0, 400, 225, sdl_window::none));
	vpl.change_scaler(200, 225, AV_PIX_FMT_YUV420P);
	CPPUNIT_ASSERT_EQUAL(video_player::horizontal_same, vpl._layout);
	CPPUNIT_ASSERT_EQUAL((size_t)100, vpl._w1);
	CPPUNIT_ASSERT_EQUAL((size_t)100, vpl._w2);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h1);
	CPPUNIT_ASSERT_EQUAL((size_t)0, vpl._h2);
	CPPUNIT_ASSERT_EQUAL((size_t)200, vpl._pw);
	CPPUNIT_ASSERT_EQUAL((size_t)225, vpl._ph);
}

namespace{
	class load_frame{
	private:
		media_stream mc;
		video_decoder vdec;
	public:
		load_frame(const char *f, video_frame &vfm)
			: mc(TRACE_PARAMS(f)),
				vdec(TRACE_PARAMS(mc, 0)){
			media_packet pkt;
			while(mc.read_packet(pkt) && !vdec.got_frame()){
				if(vdec.contains_data(pkt)){
					vdec.decode_frame(pkt, vfm);
				}
			}
		}
		load_frame(video_frame &vfm)
			: mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v")),
				vdec(TRACE_PARAMS(mc, 0)){
			media_packet pkt;
			while(mc.read_packet(pkt) && !vdec.got_frame()){
				if(vdec.contains_data(pkt)){
					vdec.decode_frame(pkt, vfm);
				}
			}
		}
	};
}

void video_player_test_class::video_player_play_video_same(){
	video_frame TRACE_CONSTRUCT(frm);
	load_frame lf(frm);
	frm.presentation_time(-100);
	video_player vpl(TRACE_PARAMS(0, 0, 192, 240, sdl_window::none, 1));
	CCSDL2::sdl_locked_txture &lt = vpl._buffer.first()->locked_txture();
	for(size_t i=0;i<lt.line()*vpl.height()*3/2;++i){
		static_cast<char*>(lt.data())[i]=rand();
	}
	
	vpl.change_scaler(192, 240, AV_PIX_FMT_YUV420P);
	vpl.push_video(frm);
	thread::sleep_nano(100*1000*1000);
	
	uint8_t *pf1 = frm._frame->data[0];
	int linef1 = frm._frame->linesize[0];
	uint8_t *pf2 = frm._frame->data[1];
	int linef2 = frm._frame->linesize[1];
	uint8_t *pf3 = frm._frame->data[2];
	int linef3 = frm._frame->linesize[2];
	
	char *ps1 = (char*)lt.data();
	char *ps2 = ps1 + lt.line()*vpl.height();
	char *ps3 = ps2 + (lt.line()/2)*(vpl.height()/2);
	for(size_t y=0; y< vpl.height(); y++){
		CPPUNIT_ASSERT(0 == memcmp(pf1, ps1, vpl.width()));
		pf1+=linef1;
		ps1+=lt.line();
	}
	for(size_t y=0; y< vpl.height(); y+=2){
		CPPUNIT_ASSERT(0 == memcmp(pf2, ps2,vpl.width()/2));
		CPPUNIT_ASSERT(0 == memcmp(pf3, ps3,vpl.width()/2));
		pf2+=linef2;
		pf3+=linef3;
		ps2+=lt.line()/2;
		ps3+=lt.line()/2;
	}
}

void video_player_test_class::video_player_play_video_v_same(){
	video_frame TRACE_CONSTRUCT(frm);
	load_frame lf(frm);
	frm.presentation_time(-100);
	video_player vpl(TRACE_PARAMS(0, 0, 192, 260, sdl_window::none, 1));
	CCSDL2::sdl_locked_txture &lt = vpl._buffer.first()->locked_txture();
	for(size_t i=0;i<lt.line()*vpl.height()*3/2;++i){
		static_cast<char*>(lt.data())[i]=rand();
	}
	vpl.change_scaler(192, 240, AV_PIX_FMT_YUV420P);
	vpl.push_video(frm);
	thread::sleep_nano(100*1000*1000);
	
	uint8_t *pf1 = frm._frame->data[0];
	int linef1 = frm._frame->linesize[0];
	uint8_t *pf2 = frm._frame->data[1];
	int linef2 = frm._frame->linesize[1];
	uint8_t *pf3 = frm._frame->data[2];
	int linef3 = frm._frame->linesize[2];
	
	uint8_t *ps1 = (uint8_t*)lt.data();
	uint8_t *ps2 = ps1 + lt.line()*vpl.height();
	uint8_t *ps3 = ps2 + (lt.line()/2)*(vpl.height()/2);
	size_t y=0;
	for(;y<10; ++y){
		for(size_t x=0; x<vpl.width(); ++x){
			CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_y(0,0,0), (int)ps1[x]);
		}
		ps1+=lt.line();
		if(!(y&1)){
			for(size_t x=0; x<vpl.width()/2; ++x){
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_u(0,0,0), (int)ps2[x]);
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_v(0,0,0), (int)ps3[x]);
			}
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
	for(;y<250;++y){
		CPPUNIT_ASSERT(0 == memcmp(pf1, ps1, vpl.width()));
		pf1+=linef1;
		ps1+=lt.line();
		if(!(y&1)){
			CPPUNIT_ASSERT(0 == memcmp(pf2, ps2,vpl.width()/2));
			CPPUNIT_ASSERT(0 == memcmp(pf3, ps3,vpl.width()/2));
			pf2+=linef2;
			pf3+=linef3;
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
	for(;y<260;++y){
		for(size_t x=0; x<vpl.width(); ++x){
			CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_y(0,0,0), (int)ps1[x]);
		}
		ps1+=lt.line();
		if(!(y&1)){
			for(size_t x=0; x<vpl.width()/2; ++x){
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_u(0,0,0), (int)ps2[x]);
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_v(0,0,0), (int)ps3[x]);
			}
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
}

void video_player_test_class::video_player_play_video_v_same_not_align(){
	video_frame TRACE_CONSTRUCT(frm);
	load_frame lf(frm);
	frm.presentation_time(-100);
	video_player vpl(TRACE_PARAMS(0, 0, 192, 250, sdl_window::none, 1));
	CCSDL2::sdl_locked_txture &lt = vpl._buffer.first()->locked_txture();
	for(size_t i=0;i<lt.line()*vpl.height()*3/2;++i){
		static_cast<char*>(lt.data())[i]=rand();
	}
	vpl.change_scaler(192, 240, AV_PIX_FMT_YUV420P);
	vpl.push_video(frm);
	thread::sleep_nano(100*1000*1000);
	
	uint8_t *pf1 = frm._frame->data[0];
	int linef1 = frm._frame->linesize[0];
	uint8_t *pf2 = frm._frame->data[1];
	int linef2 = frm._frame->linesize[1];
	uint8_t *pf3 = frm._frame->data[2];
	int linef3 = frm._frame->linesize[2];
	
	uint8_t *ps1 = (uint8_t*)lt.data();
	uint8_t *ps2 = ps1 + lt.line()*vpl.height();
	uint8_t *ps3 = ps2 + (lt.line()/2)*(vpl.height()/2);
	size_t y=0;
	for(;y<5; ++y){
		for(size_t x=0; x<vpl.width(); ++x){
			CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_y(0,0,0), (int)ps1[x]);
		}
		ps1+=lt.line();
		if(!(y&1)){
			for(size_t x=0; x<vpl.width()/2; ++x){
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_u(0,0,0), (int)ps2[x]);
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_v(0,0,0), (int)ps3[x]);
			}
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
	for(;y<245;++y){
		CPPUNIT_ASSERT(0 == memcmp(pf1, ps1, vpl.width()));
		pf1+=linef1;
		ps1+=lt.line();
		if(!(y&1)){
			CPPUNIT_ASSERT(0 == memcmp(pf2, ps2,vpl.width()/2));
			CPPUNIT_ASSERT(0 == memcmp(pf3, ps3,vpl.width()/2));
			pf2+=linef2;
			pf3+=linef3;
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
	for(;y<250;++y){
		for(size_t x=0; x<vpl.width(); ++x){
			CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_y(0,0,0), (int)ps1[x]);
		}
		ps1+=lt.line();
		if(!(y&1)){
			for(size_t x=0; x<vpl.width()/2; ++x){
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_u(0,0,0), (int)ps2[x]);
				CPPUNIT_ASSERT_EQUAL((int)CCSDL2::rgb_2_v(0,0,0), (int)ps3[x]);
			}
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
}

void video_player_test_class::video_player_play_video_h_same(){
	video_frame TRACE_CONSTRUCT(frm);
	load_frame lf(frm);
	frm.presentation_time(-100);
	video_player vpl(TRACE_PARAMS(0, 0, 200, 240, sdl_window::none, 1));
	CCSDL2::sdl_locked_txture &lt = vpl._buffer.first()->locked_txture();
	for(size_t i=0;i<lt.line()*vpl.height()*3/2;++i){
		static_cast<char*>(lt.data())[i]=rand();
	}
	
	vpl.change_scaler(192, 240, AV_PIX_FMT_YUV420P);
	vpl.push_video(frm);
	thread::sleep_nano(100*1000*1000);
	
	uint8_t *pf1 = frm._frame->data[0];
	int linef1 = frm._frame->linesize[0];
	uint8_t *pf2 = frm._frame->data[1];
	int linef2 = frm._frame->linesize[1];
	uint8_t *pf3 = frm._frame->data[2];
	int linef3 = frm._frame->linesize[2];
	uint8_t *ps1 = (uint8_t*)lt.data();
	uint8_t *ps2 = ps1 + lt.line()*vpl.height();
	uint8_t *ps3 = ps2 + (lt.line()/2)*(vpl.height()/2);
	ps1+=4;
	ps2+=2;
	ps3+=2;
	for(size_t y=0; y<vpl.height(); y++){
		CPPUNIT_ASSERT(0 == memcmp(pf1, ps1, 192));
		pf1+=linef1;
		ps1+=lt.line();
		if(!(y&1)){
			CPPUNIT_ASSERT(0 == memcmp(pf2, ps2, 96));
			CPPUNIT_ASSERT(0 == memcmp(pf3, ps3, 96));
			pf2+=linef2;
			pf3+=linef3;
			ps2+=lt.line()/2;
			ps3+=lt.line()/2;
		}
	}
}

void video_player_test_class::video_player_play_video_same_block(){
	video_frame TRACE_CONSTRUCT(frm);
	load_frame lf("/extend/rd_1/storage/users/leeonky/project/splayer_lib/media/hd_dolby_countdown_v1_lossless.m2ts", frm);
	frm.presentation_time(-100);
	video_player vpl(TRACE_PARAMS(0, 0, 1920, 1080, sdl_window::none, 1));
	CCSDL2::sdl_locked_txture &lt = vpl._buffer.first()->locked_txture();
	for(size_t i=0;i<lt.line()*vpl.height()*3/2;++i){
		static_cast<char*>(lt.data())[i]=rand();
	}
	
	vpl.change_scaler(1920, 1080, AV_PIX_FMT_YUV420P);
	vpl.push_video(frm);
	thread::sleep_nano(100*1000*1000);
	
	uint8_t *pf1 = frm._frame->data[0];
	int linef1 = frm._frame->linesize[0];
	uint8_t *pf2 = frm._frame->data[1];
	int linef2 = frm._frame->linesize[1];
	uint8_t *pf3 = frm._frame->data[2];
	int linef3 = frm._frame->linesize[2];
	
	char *ps1 = (char*)lt.data();
	char *ps2 = ps1 + lt.line()*vpl.height();
	char *ps3 = ps2 + (lt.line()/2)*(vpl.height()/2);
	for(size_t y=0; y< vpl.height(); y++){
		CPPUNIT_ASSERT(0 == memcmp(pf1, ps1, vpl.width()));
		pf1+=linef1;
		ps1+=lt.line();
	}
	for(size_t y=0; y< vpl.height(); y+=2){
		CPPUNIT_ASSERT(0 == memcmp(pf2, ps2,vpl.width()/2));
		CPPUNIT_ASSERT(0 == memcmp(pf3, ps3,vpl.width()/2));
		pf2+=linef2;
		pf3+=linef3;
		ps2+=lt.line()/2;
		ps3+=lt.line()/2;
	}
}

void video_player_test_class::video_player_seek_frm(){
	video_player vpl(TRACE_PARAMS(0, 0, 1920, 1080, sdl_window::none, 1));
	video_frame TRACE_CONSTRUCT(frm);
	frm.presentation_time(1000);
	frm.duration(30);
	vpl.push_video(frm);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), (vpl.current_clock()+200)/1000);
}
