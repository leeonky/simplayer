#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cclibc/io/file.h"
#include "cclibc/exception.h"
#include "cclibc/callc.h"

CCLIBC_BG

file::file FUNCTION_PARAMETER_LIST(const char *n, file_descriptor::mode m):
	file_descriptor(INVOKE_THROW(int, == -1, posix_error, open, n, static_cast<int>(m))){}

uint64_t file::file_size FUNCTION_PARAMETER_LIST(const char *f){
	struct stat st;
	VOID_INVOKE_THROW(== -1, posix_error, stat, f, &st);
	return st.st_size;
}

CCLIBC_END
