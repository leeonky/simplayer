#include "cctest/util.h"
#include "ccalsa/asound.cpp"

INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_open, r_non_call<int>, snd_pcm_t**, const char*, snd_pcm_stream_t, int)
INTERFACE_TESTABLE_FUNCTION_1(const char*, snd_strerror, r_non_call<const char*>, int)
INTERFACE_TESTABLE_FUNCTION_1(int, snd_pcm_close, r_non_call<int>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_3(snd_pcm_sframes_t, snd_pcm_writei, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*, const void*, snd_pcm_uframes_t)
INTERFACE_TESTABLE_FUNCTION_1(int, snd_pcm_prepare, r_non_call<int>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_1(int, snd_pcm_drop, r_non_call<int>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_1(int, snd_pcm_resume, r_non_call<int>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_1(snd_pcm_sframes_t, snd_pcm_avail, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_1(snd_pcm_sframes_t, snd_pcm_avail_update, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_1(snd_pcm_sframes_t, snd_pcm_rewindable, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_2(snd_pcm_sframes_t, snd_pcm_rewind, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*, snd_pcm_uframes_t)
INTERFACE_TESTABLE_FUNCTION_1(snd_pcm_sframes_t, snd_pcm_forwardable, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*)
INTERFACE_TESTABLE_FUNCTION_2(snd_pcm_sframes_t, snd_pcm_forward, r_non_call<snd_pcm_sframes_t>, snd_pcm_t*, snd_pcm_uframes_t)
INTERFACE_TESTABLE_FUNCTION_1(snd_pcm_state_t, snd_pcm_state, r_non_call<snd_pcm_state_t>, snd_pcm_t*)

namespace{
	int snd_pcm_wait_mock(...){
		return 1;
	}
}

INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_wait, snd_pcm_wait_mock, snd_pcm_t*, int)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_delay, r_non_call<int>, snd_pcm_t*, snd_pcm_sframes_t*)

INTERFACE_TESTABLE_FUNCTION_1(int, snd_pcm_hw_params_malloc, r_non_call<int>, snd_pcm_hw_params_t**)
INTERFACE_TESTABLE_VOID_FUNCTION_1(snd_pcm_hw_params_free, non_call, snd_pcm_hw_params_t*)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_any, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*)
INTERFACE_TESTABLE_FUNCTION(size_t, snd_pcm_hw_params_sizeof, r_non_call<size_t>)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_get_access, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_access_t*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_access, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_access_t)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_get_format, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_format_t*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_format, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_format_t)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_test_format, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_format_t)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_get_channels, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_channels, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_channels_near, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_rate, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_rate, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned, int)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_rate_near, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_period_size, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_uframes_t*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_size, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t, int)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_size_near, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_size_max, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_period_size_max, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_uframes_t*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_size_min, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_period_size_min, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_uframes_t*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_period_time, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_time, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned, int)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_time_near, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_period_time_max, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_time_max, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_period_time_min, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_period_time_min, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)

INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_buffer_size, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_get_buffer_size, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_buffer_size_near, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_buffer_size_max, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_get_buffer_size_max, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_set_buffer_size_min, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params_get_buffer_size_min, r_non_call<int>, const snd_pcm_hw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_buffer_time, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned, int)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_buffer_time, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_buffer_time_near, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_buffer_time_max, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_buffer_time_max, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_4(int, snd_pcm_hw_params_set_buffer_time_min, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_hw_params_get_buffer_time_min, r_non_call<int>, const snd_pcm_hw_params_t*, unsigned*, int*)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_hw_params, r_non_call<int>, snd_pcm_t*, snd_pcm_hw_params_t*)

INTERFACE_TESTABLE_FUNCTION_1(int, snd_pcm_sw_params_malloc, r_non_call<int>, snd_pcm_sw_params_t**)
INTERFACE_TESTABLE_VOID_FUNCTION_1(snd_pcm_sw_params_free, non_call, snd_pcm_sw_params_t*)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_sw_params_current, r_non_call<int>, snd_pcm_t*, snd_pcm_sw_params_t*)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_sw_params_get_avail_min, r_non_call<int>, const snd_pcm_sw_params_t*, snd_pcm_uframes_t*)
INTERFACE_TESTABLE_FUNCTION_3(int, snd_pcm_sw_params_set_avail_min, r_non_call<int>, snd_pcm_t*, snd_pcm_sw_params_t*, snd_pcm_uframes_t)
INTERFACE_TESTABLE_FUNCTION_2(int, snd_pcm_sw_params, r_non_call<int>, snd_pcm_t*, snd_pcm_sw_params_t*)