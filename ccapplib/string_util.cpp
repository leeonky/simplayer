#include <ctype.h>
#include "ccapplib/string_util.h"

CCAPPLIB_BG

bool string_util::end_with(const char *str, const char *s){
	size_t len1 = strlen(str);
	size_t len2 = strlen(s);
	if(len2>len1){
		return false;
	}
	for(size_t len = std::min(len1, len2); len>0; --len){
		if(str[len1 - len] != s[len2 - len]){
			return false;
		}
	}
	return true;
}

bool string_util::end_with_ig(const char *str, const char *s){
	size_t len1 = strlen(str);
	size_t len2 = strlen(s);
	if(len2>len1){
		return false;
	}
	for(size_t len = std::min(len1, len2); len>0; --len){
		if(toupper(str[len1 - len]) != toupper(s[len2 - len])){
			return false;
		}
	}
	return true;
}

CCAPPLIB_END
