#include "cctest/util.h"
#include "ccmedia/ass_decoder.cpp"

INTERFACE_TESTABLE_FUNCTION(ASS_Library*, ass_library_init, r_non_call_value<ASS_Library*>)
INTERFACE_TESTABLE_VOID_FUNCTION_1(ass_library_done, non_call, ASS_Library*)

INTERFACE_TESTABLE_FUNCTION_1(ASS_Renderer*, ass_renderer_init, r_non_call_value<ASS_Renderer*>, ASS_Library*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(ass_renderer_done, non_call, ASS_Renderer*)

INTERFACE_TESTABLE_VOID_FUNCTION_3(ass_set_frame_size, non_call, ASS_Renderer*, int, int)
INTERFACE_TESTABLE_VOID_FUNCTION_6(ass_set_fonts, non_call, ASS_Renderer*, const char*, const char*, int, const char*, int)

INTERFACE_TESTABLE_FUNCTION_3(ASS_Track*, ass_read_file, r_non_call_value<ASS_Track*>, ASS_Library*, char*, char*)
INTERFACE_TESTABLE_FUNCTION_1(ASS_Track*, ass_new_track, r_non_call_value<ASS_Track*>, ASS_Library*)
INTERFACE_TESTABLE_VOID_FUNCTION_3(ass_process_codec_private, non_call, ASS_Track*, char*, int)
INTERFACE_TESTABLE_VOID_FUNCTION_5(ass_process_chunk, non_call, ASS_Track*, char*, int, long long, long long)
INTERFACE_TESTABLE_FUNCTION_4(ASS_Image*, ass_render_frame, r_non_call_value<ASS_Image*>, ASS_Renderer*, ASS_Track*, long long, int*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(ass_free_track, non_call, ASS_Track*)