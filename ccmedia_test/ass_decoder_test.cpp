#include "ccmedia_test/ass_decoder_test.h"
#include "ccmedia/ass_decoder.h"

using CCMEDIA::ass_decoder;

void ass_decoder_test_class::decoder_init(){
	ass_decoder ass(TRACE_PARAMS(200, 100, 200, 100));
	CPPUNIT_ASSERT(ass._lib);
	CPPUNIT_ASSERT(ass._renderer);
}

