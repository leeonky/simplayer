#include "cclibc_test/env_test.h"
#include "cclibc/env.h"

using CCLIBC::util;

void env_test_class::algin_test() {
	for(size_t i = 0; i < 16; i++){
		size_t v = util::algin<size_t, sizeof(int*)>(i);
		CPPUNIT_ASSERT(v == (i+sizeof(int*)-1)/sizeof(int*)*sizeof(int*));
	}
}

void env_test_class::algin_macro_test() {
	for(size_t i = 0; i < 16; i++){
		size_t v = SIZE_ALGIN(i, sizeof(int*));
		CPPUNIT_ASSERT(v == (i+sizeof(int*)-1)/sizeof(int*)*sizeof(int*));
	}
}
