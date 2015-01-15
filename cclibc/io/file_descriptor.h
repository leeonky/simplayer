#ifndef CCLIBC_IO_FILE_DESCRIPTOR_H_
#define CCLIBC_IO_FILE_DESCRIPTOR_H_

#include <fcntl.h>
#include "cclibc/env.h"
#include "cclibc/trace.h"

CCLIBC_BG

class file_descriptor{
private:
	int _fd;
public:
	enum mode{
		readonly = O_RDONLY,
		writeonly = O_WRONLY,
		readwrite = O_RDWR,
		truncated = O_TRUNC,
		create = O_CREAT,
	};

	file_descriptor(int fd);
	int value() const{
		return _fd;
	}
	virtual ~file_descriptor();
	
	size_t block_write FUNCTION_PARAMETER_LIST(const void*, size_t);
	size_t block_read FUNCTION_PARAMETER_LIST(void*, size_t);
	uint64_t seek_set FUNCTION_PARAMETER_LIST(int64_t);
};

inline file_descriptor::mode operator| (file_descriptor::mode m1, file_descriptor::mode m2){
		return static_cast<file_descriptor::mode>(static_cast<int>(m1)|static_cast<int>(m2));
}

#ifdef STACK_TRACE_FLAG

#define block_write(...) block_write(INVOKE_TRACE, ##__VA_ARGS__)
#define block_read(...) block_read(INVOKE_TRACE, ##__VA_ARGS__)
#define seek_set(...) seek_set(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCLIBC_END

#endif // CCLIBC_IO_FILE_DESCRIPTOR_H_
