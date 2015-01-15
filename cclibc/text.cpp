#include <cstdio>
#include <iconv.h>
#include "cclibc/text.h"
#include "cclibc/callc.h"
#include "cclibc/io/file.h"
#include "cclibc/exception.h"

CCLIBC_BG

size_t encoding::transform_format FUNCTION_PARAMETER_LIST(const char *sset, const char *dset, const void *src, size_t sl, void *dst, size_t dl){
	iconv_t i = iconv_open(dset, sset);
	if((iconv_t)-1 == i){
		THROW_NEW_EXCEPTION(posix_error, "iconv_open", posix_error::error());
	}
	try{
		size_t tdl = dl;
		char *psrc = (char*)src;
		char *pdst = static_cast<char*>(dst);
		if(static_cast<size_t>(-1) == iconv(i, &psrc, &sl, &pdst, &dl)){
			THROW_NEW_EXCEPTION(posix_error, "iconv", posix_error::error());
		}
		iconv_close(i);
		return tdl - dl;
	}catch(...){
		iconv_close(i);
		throw;
	}
}

namespace{
	__thread char encoding_name[128];
	
}

const char* encoding::detect_encoding FUNCTION_PARAMETER_LIST(const char *f){
	char buf[4096];
	snprintf(buf, sizeof(buf), "file -bi '%s' | grep charset | awk -F= '{print $2}'", f);
	FILE *fp = INVOKE_THROW(FILE*, == NULL, posix_error, popen, buf, "r");
	*encoding_name = '\0';
	fgets(encoding_name, sizeof(encoding_name), fp);
	pclose(fp);
	return encoding_name;
}

const char* encoding::detect_encoding FUNCTION_PARAMETER_LIST(const char *f, size_t c, const char **cs){
	stack_pool pool;
	size_t os;
	void *org_txt = file::read_all_text(pool, f, os);
	size_t s = os*3;
	void *buf1 = pool.allocate(s), *buf2 = pool.allocate(s);
	for(size_t i=0; i<c; ++i){
		const char *m = strcmp(cs[i], "utf8") ? cs[i] : "utf32";
		try{
			size_t s1 = transform_format(cs[i], m, org_txt, os, buf1, s);
			s1 = transform_format(m, cs[i], buf1, s1, buf2, s);
			if(!memcmp(buf2, org_txt, os)){
				return cs[i];
			}
		}catch(...){}
	}
	return NULL;
}

CCLIBC_END