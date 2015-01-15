#include "cctest/util.h"
#include "cclibc_test/env_test.h"
#include "cclibc_test/trace_test.h"
#include "cclibc_test/util/list_test.h"
#include "cclibc_test/memory_test.h"
#include "cclibc_test/exception_test.h"
#include "cclibc_test/callc_test.h"
#include "cclibc_test/thread/thread_test.h"
#include "cclibc_test/thread/mutex_test.h"
#include "cclibc_test/thread/semaphore_test.h"
#include "cclibc_test/thread/condition_test.h"
#include "cclibc_test/util/avl_tree_test.h"
#include "cclibc_test/util/map_test.h"
#include "cclibc_test/util/array_list_test.h"
#include "cclibc_test/io/file_descriptor_test.h"
#include "cclibc_test/io/file_test.h"
#include "cctest/util.h"
#include "ut.h"

int main() {
	return do_all_test("CCLIBC TESTING");
}
