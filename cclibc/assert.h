#ifndef CCLIBC_ASSERT_H_
#define CCLIBC_ASSERT_H_

#include "cclibc/env.h"

CCLIBC_BG

class assert_util{
public:
	static int assert_print(const char*, const char*, const char*, int);
};

CCLIBC_END

#ifndef NDBUG
#define debug_assert(exp) release_assert(exp)
#else
#define debug_assert(exp)
#endif

#define release_assert(expression) (void)((expression) || !CCLIBC::assert_util::assert_print(#expression, __PRETTY_FUNCTION__, __FILE__, __LINE__))

#endif
