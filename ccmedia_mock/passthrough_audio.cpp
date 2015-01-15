#include "cctest/util.h"
#include "ccmedia/passthrough_audio.cpp"

typedef int (*FUN_IO)(void*, uint8_t*, int);
typedef int64_t (*FUN_SEEK)(void*, int64_t, int);

INTERFACE_TESTABLE_FUNCTION_7(AVIOContext*, avio_alloc_context, r_non_call<AVIOContext*>, unsigned char*, int, int, void*, FUN_IO, FUN_IO, FUN_SEEK)
INTERFACE_TESTABLE_VOID_FUNCTION_1(av_free, non_call, void*)
INTERFACE_TESTABLE_FUNCTION_4(int, av_opt_set_int, r_non_call<int>, void*, const char*, int64_t, int)
INTERFACE_TESTABLE_FUNCTION_4(int, avformat_alloc_output_context2, r_non_call<int>, AVFormatContext**, AVOutputFormat*, const char*, const char*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(avformat_free_context, non_call, AVFormatContext*)