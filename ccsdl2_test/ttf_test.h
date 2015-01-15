#ifndef CCSDL2_TTF_TEST_H
#define CCSDL2_TTF_TEST_H

#include "cctest/util.h"

class ttf_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(ttf_test_class);
	CPPUNIT_TEST(sdlttf_component_init);
	CPPUNIT_TEST(sdlttf_font_outline);
	CPPUNIT_TEST(surface_size_properties);
	CPPUNIT_TEST(sdlttf_font_style);
	CPPUNIT_TEST(rich_text_renderer_new);
	CPPUNIT_TEST(rich_text_renderer_change_size);
	CPPUNIT_TEST_SUITE_END();
	
	void sdlttf_component_init();
	void sdlttf_font_outline();
	void surface_size_properties();
	void sdlttf_font_style();
	void rich_text_renderer_new();
	void rich_text_renderer_change_size();
};


#endif //CCSDL2_TTF_TEST_H

