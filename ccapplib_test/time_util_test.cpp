#include "ccapplib_test/time_util_test.h"
#include "ccapplib/time_util.h"

using CCAPPLIB::time_bucket;

void time_util_test_class::time_bucket_create(){
	time_bucket t1(1,2);
	CPPUNIT_ASSERT(t1.start() == 1);
	CPPUNIT_ASSERT(t1.end() == 3);
}
void time_util_test_class::time_bucket_contains(){
	time_bucket t1(1,2);
	CPPUNIT_ASSERT(!t1.contains(0));
	CPPUNIT_ASSERT(t1.contains(1));
	CPPUNIT_ASSERT(t1.contains(2));
	CPPUNIT_ASSERT(!t1.contains(3));

}