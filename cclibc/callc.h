#ifndef CCLIBC_CALLC_H_
#define CCLIBC_CALLC_H_

#include "cclibc/env.h"
#include "cclibc/exception.h"

#define INVOKE_THROW(ret_type, failed, error_exp, function, ...) ({ret_type _r_tmp = function(__VA_ARGS__);\
	if(_r_tmp failed)\
		THROW_NEW_EXCEPTION(error_exp, #function);\
	_r_tmp;\
})

#define INVOKE_THROW_RETURN(ret_type, failed, error_exp, function, ...) ({ret_type _r_tmp = function(__VA_ARGS__);\
	if(_r_tmp failed)\
		THROW_NEW_EXCEPTION(error_exp, #function, _r_tmp);\
	_r_tmp;\
})

#define VOID_INVOKE_THROW(failed, error_exp, function, ...) do{\
	if(function(__VA_ARGS__) failed){\
		THROW_NEW_EXCEPTION(error_exp, #function);\
	}\
}while(0)

#endif //CCLIBC_CALLC_H_
