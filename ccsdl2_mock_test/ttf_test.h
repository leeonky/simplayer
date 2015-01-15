#ifndef CCSDL2_TTF_TEST_H
#define CCSDL2_TTF_TEST_H

#include "cctest/util.h"

class ttf_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(ttf_test_class);
	CPPUNIT_TEST(sdlttf_component_init);
	CPPUNIT_TEST(sdlttf_font_init);
	CPPUNIT_TEST(sdlttf_font_outline);
	CPPUNIT_TEST(sdlttf_font_surface_solid);
	CPPUNIT_TEST(sdlttf_font_surface_shaded);
	CPPUNIT_TEST(sdlttf_font_surface_blended);
	CPPUNIT_TEST(sdlttf_font_surface_blended2);
	CPPUNIT_TEST(sdlttf_font_surface_solid_uni);
	CPPUNIT_TEST(sdlttf_font_surface_shaded_uni);
	CPPUNIT_TEST(sdlttf_font_surface_blended_uni);
	CPPUNIT_TEST(sdlttf_font_surface_blended2_uni);
	CPPUNIT_TEST(surface_blit_to);
	CPPUNIT_TEST(surface_blit_to_sr);
	CPPUNIT_TEST(surface_blit_to_dr);
	CPPUNIT_TEST(surface_blit_to_sr_dr);
	CPPUNIT_TEST(sdlttf_font_size_of);
	CPPUNIT_TEST(sdlttf_font_line_height);
	CPPUNIT_TEST(texture_init_from_surface);
	CPPUNIT_TEST(sdlttf_change_style);
	CPPUNIT_TEST(rich_text_renderer_new_font_mgt);
	CPPUNIT_TEST(rich_text_renderer_change_size);
	CPPUNIT_TEST(sdl_surface_copy);
	//{\1c&H00FFFC&}{\fs16}<b><i><u>(Underline) <font color="color name or #code"> <br> \n
	CPPUNIT_TEST(rich_text_renderer_ass_change_color);
	CPPUNIT_TEST(rich_text_renderer_ass_change_size);
	CPPUNIT_TEST(rich_text_renderer_srt_bold);
	CPPUNIT_TEST(rich_text_renderer_srt_italic);
	CPPUNIT_TEST(rich_text_renderer_srt_underline);
	CPPUNIT_TEST(rich_text_renderer_srt_font_color);
	CPPUNIT_TEST(rich_text_renderer_srt_br);
	CPPUNIT_TEST_SUITE_END();
	
	void sdlttf_component_init();
	void sdlttf_font_init();
	void sdlttf_font_outline();
	void sdlttf_font_surface_solid();
	void sdlttf_font_surface_shaded();
	void sdlttf_font_surface_blended();
	void sdlttf_font_surface_blended2();
	void sdlttf_font_surface_solid_uni();
	void sdlttf_font_surface_shaded_uni();
	void sdlttf_font_surface_blended_uni();
	void sdlttf_font_surface_blended2_uni();
	void surface_blit_to();
	void surface_blit_to_sr();
	void surface_blit_to_dr();
	void surface_blit_to_sr_dr();
	void sdlttf_font_size_of();
	void sdlttf_font_line_height();
	void texture_init_from_surface();
	void sdlttf_change_style();
	void rich_text_renderer_new_font_mgt();
	void rich_text_renderer_change_size();
	void sdl_surface_copy();
	void rich_text_renderer_ass_change_color();
	void rich_text_renderer_ass_change_size();
	void rich_text_renderer_srt_bold();
	void rich_text_renderer_srt_italic();
	void rich_text_renderer_srt_underline();
	void rich_text_renderer_srt_font_color();
	void rich_text_renderer_srt_br();
};


#endif //CCSDL2_TTF_TEST_H

