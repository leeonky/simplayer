#include <cstdarg>
#include "cclibc/text.h"
#include "ccapplib_test/string_util_test.h"
#include "ccapplib/string_util.h"

using CCLIBC::encoding;
using CCAPPLIB::string_util;
using CCAPPLIB::string_array;

void string_util_test_class::utf2uni(){
	char buf[128];
	for(uint16_t u16 = 0; u16<0xD800; ++u16){
		buf[encoding::transform_format("UTF-16LE", "UTF-8", &u16, sizeof(uint16_t), buf, sizeof(buf))] = '\0';
		size_t s = strlen(buf);
		size_t l = s;
		if(s){
			const char *p = buf;
			uint16_t c = string_util::uni16_char(p, s);
			CPPUNIT_ASSERT(c == u16);
			CPPUNIT_ASSERT(p+s == buf+l);
		}
	}
}

namespace{
	template<typename T>
	size_t text_width(T, const char *str){
		size_t r = 0;
		if(str){
			size_t len = strlen(str);
			uint16_t* buf = (uint16_t*)alloca((len+1) * sizeof(uint16_t));
			size_t u16_len = encoding::transform_format("UTF-8", "UTF-16LE", str, len, buf, len * sizeof(uint16_t))/2;
			buf[u16_len]=0;
			for(size_t i=0; i<u16_len && buf[i]; i++){
				if(buf[i]<=0x9FA5 && buf[i]>=0x4E00){
					r+=2;
				}else{
					++r;
				}
			}
		}
		return r;
	}

	void wrap_wrap FUNCTION_PARAMETER_LIST(const char *str, size_t w, size_t line,...){
		string_array<> lines;
		string_util::split_line_by_size(text_width<int>, 0, str, w, lines);
		
		try{
		CPPUNIT_ASSERT(lines.size() == line);

		va_list list;
		va_start(list, line);
		for(size_t i=0; i < line; i++){
			char *tstr = va_arg(list, char*);
			CPPUNIT_ASSERT_EXT(!strcmp(lines[i], tstr), "\n%s-%s\n", lines[i], tstr);
		}
		va_end(list);
		}catch(...){
			printf("\nLine count %d:\n", (int)lines.size());
			for(size_t j=0; j < lines.size(); j++){
				printf("\t%s\n", lines[j]);
			}
			throw;
		}
	}

	void wrap_wrap FUNCTION_PARAMETER_LIST(const char *str, size_t line,...){
		string_array<> lines;
		string_util::split_line(str, lines);
		try{
		CPPUNIT_ASSERT(lines.size() == line);

		va_list list;
		va_start(list, line);
		for(size_t i=0; i < line; i++){
			char *tstr = va_arg(list, char*);
			CPPUNIT_ASSERT_EXT(!strcmp(lines[i], tstr), "\n%s-%s\n", lines[i], tstr);
		}
		va_end(list);
		}catch(...){
			printf("\nLine count %d:\n", (int)lines.size());
			for(size_t j=0; j < lines.size(); j++){
				printf("\t%s\n", lines[j]);
			}
			throw;
		}
	}
};

#ifdef STACK_TRACE_FLAG

#define wrap_wrap(...) wrap_wrap(INVOKE_TRACE, ##__VA_ARGS__)

#endif

void string_util_test_class::wrap_empty(){
	string_array<> lines;
	string_util::split_line_by_size(text_width<int>, 0, "", 1, lines);
	CPPUNIT_ASSERT(!lines.size());
}

void string_util_test_class::wrap_no_wrap_1_char(){
	wrap_wrap("a", 4, 1, "a");
}

void string_util_test_class::string_clear(){
	{
		test_util_allocator::reset();
		string_array<test_util_allocator> lines;
		string_util::split_line_by_size(text_width<int>, 0, "1", 2, lines);
		CPPUNIT_ASSERT(test_util_allocator::allocated());
	}
	CPPUNIT_ASSERT(!test_util_allocator::allocated());
}

void string_util_test_class::wrap_no_wrap(){
	wrap_wrap("abcd", 4, 1, "abcd");
	wrap_wrap("你好", 4, 1, "你好");
	wrap_wrap("a你d", 4, 1, "a你d");
}

void string_util_test_class::wrap_no_wrap_too_long(){
	wrap_wrap("abcdedf", 4, 1, "abcdedf");
}

void string_util_test_class::wrap_only_stopper_letter(){
	wrap_wrap("abc,ef", 4, 2, "abc,", "ef");
}

void string_util_test_class::wrap_stopper_cjk(){
	wrap_wrap("你好你1", 4, 2, "你好", "你1");
}

void string_util_test_class::wrap_stopper_letters(){
	wrap_wrap("abc,efg,hi", 4, 3, "abc,", "efg,", "hi");
}

void string_util_test_class::wrap_stopper_cjks(){
	wrap_wrap("你好张三李四王1", 4, 4, "你好", "张三", "李四", "王1");
}

void string_util_test_class::wrap_stopper_letters_too_long(){
	wrap_wrap("abc,efgssss,hiasdsad", 4, 3, "abc,", "efgssss,", "hiasdsad");
}

void string_util_test_class::wrap_stopper_cjks_too_long(){
	wrap_wrap("你好a张bcd李四王1", 4, 5, "你好", "a张", "bcd李", "四王", "1");
}

void string_util_test_class::wrap_stopper_last_one_stopper(){
	wrap_wrap("你好你", 4, 2, "你好", "你");
}

void string_util_test_class::wrap_letter_space(){
	wrap_wrap("abcd efg", 4, 2, "abcd", "efg");
}

void string_util_test_class::wrap_cjk_space(){
	wrap_wrap("你好 张23 123", 4, 3, "你好", "张23", "123");
}

void string_util_test_class::wrap_letter_n(){
	wrap_wrap("a\r\nb", 4, 2, "a", "b");
	wrap_wrap("a\nb", 4, 2, "a", "b");
	wrap_wrap("a\rb", 4, 2, "a", "b");
	wrap_wrap("a\r\n\nb", 4, 3, "a", "", "b");
}

void string_util_test_class::splite_line_r(){
	wrap_wrap("", 1, "");
	wrap_wrap("你好\r张23\r123", 3, "你好", "张23", "123");
}

void string_util_test_class::splite_line_n(){
	wrap_wrap("你好\n张23\n123", 3, "你好", "张23", "123");
}

void string_util_test_class::splite_line_rn(){
	wrap_wrap("你好\r\n张23\r123", 3, "你好", "张23", "123");
	wrap_wrap("你好\r\n张23\n123", 3, "你好", "张23", "123");
}

void string_util_test_class::string_end_with(){
	CPPUNIT_ASSERT(string_util::end_with("", ""));
	CPPUNIT_ASSERT(string_util::end_with("1", ""));
	CPPUNIT_ASSERT(!string_util::end_with("", "1"));
	CPPUNIT_ASSERT(string_util::end_with("ab", "b"));
	CPPUNIT_ASSERT(!string_util::end_with("ab", "0ab"));
}

void string_util_test_class::string_end_with_i(){
	CPPUNIT_ASSERT(string_util::end_with_ig("", ""));
	CPPUNIT_ASSERT(string_util::end_with_ig("1", ""));
	CPPUNIT_ASSERT(!string_util::end_with_ig("", "1"));
	CPPUNIT_ASSERT(string_util::end_with_ig("ab", "B"));
	CPPUNIT_ASSERT(!string_util::end_with_ig("ab", "0ab"));
}

