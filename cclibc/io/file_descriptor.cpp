#include <unistd.h>
#include "cclibc/io/file_descriptor.h"
#include "cclibc/exception.h"
#include "cclibc/callc.h"

CCLIBC_BG

file_descriptor::file_descriptor(int fd): _fd(fd){
}

file_descriptor::~file_descriptor(){
	close(_fd);
}

size_t file_descriptor::block_write FUNCTION_PARAMETER_LIST(const void *buf, size_t len){
	return INVOKE_THROW(int, < 0, posix_error, write, _fd, buf, len);
}

size_t file_descriptor::block_read FUNCTION_PARAMETER_LIST(void *buf, size_t len){
	return INVOKE_THROW(int, < 0, posix_error, read, _fd, buf, len);
}

uint64_t file_descriptor::seek_set FUNCTION_PARAMETER_LIST(int64_t pos){
	return INVOKE_THROW(int, < 0, posix_error, lseek, _fd, pos, SEEK_SET);
}

CCLIBC_END
