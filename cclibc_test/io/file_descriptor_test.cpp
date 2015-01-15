#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cclibc_test/io/file_descriptor_test.h"
#include "cclibc/io/file_descriptor.h"

using CCLIBC::file_descriptor;

namespace{
	class file_des: public file_descriptor{
	public:
		file_des(): file_descriptor(open("/tmp/a", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU)){}
	};
};

void file_descriptor_test_class::file_descriptor_close(){
	int i;
	{
		file_des fd;
		i = fd._fd;
		CPPUNIT_ASSERT(!fcntl(i, F_GETFD));
	}
	CPPUNIT_ASSERT(fcntl(i, F_GETFD));
}

void file_descriptor_test_class::file_descriptor_block_read_write(){
	file_des fd;
	const char *str = "Hello 123";
	char buf[128];
	fd.block_write(str, strlen(str)+1);
	fd.seek_set(0);
	fd.block_read(buf, strlen(str)+1);
	CPPUNIT_ASSERT(!strcmp(buf, str));
}
