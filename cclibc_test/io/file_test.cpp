#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cclibc_test/io/file_test.h"
#include "cclibc/io/file.h"

using CCLIBC::file;
using CCLIBC::file_descriptor;
using CCLIBC::pool_ptr;
using CCLIBC::allocator;

void file_test_class::file_close(){
	int fd;
	{
		file f(TRACE_PARAMS("/tmp/a", file_descriptor::readwrite | file_descriptor::create | file_descriptor::truncated));
		fd=f._fd;
		CPPUNIT_ASSERT(!fcntl(f._fd, F_GETFD));
	}
	CPPUNIT_ASSERT(fcntl(fd, F_GETFD));
}

void file_test_class::file_block_read_write(){
	file f(TRACE_PARAMS("/tmp/a", file_descriptor::readwrite | file_descriptor::create | file_descriptor::truncated));
	const char *str = "Hello 123";
	char buf[128];
	f.block_write(str, strlen(str)+1);
	f.seek_set(0);
	f.block_read(buf, strlen(str)+1);
	CPPUNIT_ASSERT(!strcmp(buf, str));
}

void file_test_class::get_file_size(){
	const char *str = "Hello 123";
	{
		file f(TRACE_PARAMS("/tmp/a", file_descriptor::readwrite | file_descriptor::create | file_descriptor::truncated));
		f.block_write(str, strlen(str)+1);
	}
	CPPUNIT_ASSERT(file::file_size("/tmp/a") == strlen(str)+1);
}

void file_test_class::read_all_file(){
	const char *str = "Hello 123";
	{
		file f(TRACE_PARAMS("/tmp/a", file_descriptor::readwrite | file_descriptor::create | file_descriptor::truncated));
		f.block_write(str, strlen(str)+1);
	}
	pool_ptr<allocator> ptr(file::read_all<allocator>(TRACE_PARAMS("/tmp/a")));
	CPPUNIT_ASSERT(!strcmp((char*)ptr.raw_pointer(), str));
}
