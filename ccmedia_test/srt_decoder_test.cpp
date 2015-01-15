#include "ccmedia_test/srt_decoder_test.h"
#include "ccmedia/srt_decoder.h"
#include "ccsdl2/video.h"

using CCMEDIA::srt_decoder;
using CCMEDIA::srt_stream;
using CCSDL2::sdl_color;
using CCSDL2::sdl_locked_txture;
using CCSDL2::sdl_video;
using CCSDL2::sdl_window;
using CCSDL2::sdl_renderer;
using CCSDL2::sdl_texture;
using CCAPPLIB::time_bucket;

void srt_decoder_test_class::srt_decoder_new_empty(){
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(srtdec);
	CPPUNIT_ASSERT(!stm._contents.size());
	CPPUNIT_ASSERT(!stm._max_len);
	CPPUNIT_ASSERT(!stm._last_event);
}

void srt_decoder_test_class::srt_decoder_extend_1(){
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(srtdec);
	stm.extend_content("Hello1", time_bucket(1, 1));
	CPPUNIT_ASSERT(1 == stm._contents.size());
	CPPUNIT_ASSERT(stm._contents.first()->value().size() == 1);
	CPPUNIT_ASSERT(!strcmp(stm._contents.first()->value()[0], "Hello1"));
	CPPUNIT_ASSERT(6 == stm._max_len);

	stm.extend_content("Hello22", time_bucket(3, 1));
	CPPUNIT_ASSERT(2 == stm._contents.size());
	CPPUNIT_ASSERT(stm._contents.first()->key().start() == 1);
	CPPUNIT_ASSERT(stm._contents.last()->key().start() == 3);
	CPPUNIT_ASSERT(!strcmp(stm._contents.last()->value()[0], "Hello22"));
	CPPUNIT_ASSERT(7 == stm._max_len);
}

void srt_decoder_test_class::srt_decoder_extend_same(){
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(srtdec);
	stm.extend_content("Hello1", time_bucket(1, 1));
	stm.extend_content("Hello22", time_bucket(1, 1));
	CPPUNIT_ASSERT(1 == stm._contents.size());
	CPPUNIT_ASSERT(stm._contents.first()->value().size() == 2);
	CPPUNIT_ASSERT(!strcmp(stm._contents.first()->value()[0], "Hello1"));
	CPPUNIT_ASSERT(!strcmp(stm._contents.first()->value()[1], "Hello22"));
	CPPUNIT_ASSERT(7 == stm._max_len);
}

namespace{
	void prepare_srtfile(){
		system("echo 1 > /tmp/test.srt");
		system("echo '00:00:01,000 --> 00:00:01,001' >> /tmp/test.srt");
		system("echo hello1 >> /tmp/test.srt");
		system("echo '' >> /tmp/test.srt");

		system("echo 2 >> /tmp/test.srt");
		system("echo '00:00:02,000 --> 00:00:02,001' >> /tmp/test.srt");
		system("echo hello22 >> /tmp/test.srt");
		system("echo '' >> /tmp/test.srt");
	}
}

void srt_decoder_test_class::srt_decoder_new_with_file(){
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt"));
	CPPUNIT_ASSERT(7 == stm._max_len);
	
	CPPUNIT_ASSERT(2 == stm._contents.size());
	CPPUNIT_ASSERT(stm._contents.first()->value().size() == 1);
	CPPUNIT_ASSERT(!strcmp(stm._contents.first()->value()[0], "hello1"));
	CPPUNIT_ASSERT(stm._contents.first()->key().start() == 1000);
	CPPUNIT_ASSERT(stm._contents.first()->key().end() == 1001);
	
	CPPUNIT_ASSERT(stm._contents.last()->value().size() == 1);
	CPPUNIT_ASSERT(!strcmp(stm._contents.last()->value()[0], "hello22"));
	CPPUNIT_ASSERT(stm._contents.last()->key().start() == 2000);
	CPPUNIT_ASSERT(stm._contents.last()->key().end() == 2001);
}

void srt_decoder_test_class::srt_decoder_first_render_nothing(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt"));
	sdl_locked_txture ltxt(TRACE_PARAMS(txture));
	CPPUNIT_ASSERT(!stm._last_event);
	stm.render_frame(ltxt, 0);
	CPPUNIT_ASSERT(!stm._last_event);	
}

void srt_decoder_test_class::srt_decoder_first_render(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt"));
	sdl_locked_txture ltxt(TRACE_PARAMS(txture));
	stm.render_frame(ltxt, 1000);
	CPPUNIT_ASSERT(stm._last_event);	
	CPPUNIT_ASSERT(stm._last_imgs.size() == 1);
}

void srt_decoder_test_class::srt_decoder_first_render_gbk(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt", "GBK"));
	sdl_locked_txture ltxt(TRACE_PARAMS(txture));
	stm.render_frame(ltxt, 1000);
	CPPUNIT_ASSERT(stm._last_event);	
	CPPUNIT_ASSERT(stm._last_imgs.size() == 1);
}

void srt_decoder_test_class::srt_decoder_render_nothing(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt", "GBK"));
	sdl_locked_txture ltxt(TRACE_PARAMS(txture));
	stm.render_frame(ltxt, 1000);
	stm.render_frame(ltxt, 3000);
	CPPUNIT_ASSERT(!stm._last_event);
	CPPUNIT_ASSERT(!stm._last_imgs.size());
}

void srt_decoder_test_class::srt_decoder_render_again(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt"));
	sdl_locked_txture ltxt(TRACE_PARAMS(txture));
	stm.render_frame(ltxt, 1000);
	stm.render_frame(ltxt, 1000);
	CPPUNIT_ASSERT(stm._last_event);	
	CPPUNIT_ASSERT(stm._last_imgs.size() == 1);
}

void srt_decoder_test_class::srt_decoder_render_new(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	prepare_srtfile();
	srt_decoder srtdec(TRACE_PARAMS("/usr/share/fonts/truetype/DroidSansFallback.ttf", 70, sdl_color(255,255,255)));
	srt_stream stm(TRACE_PARAMS(srtdec, "/tmp/test.srt"));
	sdl_locked_txture ltxt(TRACE_PARAMS(txture));
	stm.render_frame(ltxt, 1000);
	void *p1 = stm._last_event;
	void *p2 = &stm._last_imgs[0];
	stm.render_frame(ltxt, 2000);
	CPPUNIT_ASSERT(stm._last_event);	
	CPPUNIT_ASSERT(stm._last_imgs.size() == 1);
	CPPUNIT_ASSERT(p1 != stm._last_event);	
	CPPUNIT_ASSERT(p2 != &stm._last_imgs[0]);
}
