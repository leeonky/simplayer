#include "cclibc_test/callc_test.h"
#include "cclibc/callc.h"

using CCLIBC::posix_error;

static int return_error(int c){
	return c;
}

static int return_no_error(int){
	return 0;
}

static int return_error_21(int c, int){
	return c;
}

static int return_no_error_2(int, int){
	return 0;
}

static int return_error_22(int, int c){
	return c;
}

void callc_test_class::return_error_1(){
	try{
		INVOKE_THROW_RETURN(int, != 0, posix_error, return_error, 1);
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == 1);
	}
	try{
		INVOKE_THROW_RETURN(int, != 0, posix_error, return_no_error, 1);
	}catch(...){
		CPPUNIT_ASSERT(!"Should not be here.");
	}
}

void callc_test_class::return_error_2(){
	try{
		INVOKE_THROW_RETURN(int, != 0, posix_error, return_error_21, 1, 2);
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == 1);
	}
	try{
		INVOKE_THROW_RETURN(int, != 0, posix_error, return_no_error_2, 1, 1);
	}catch(...){
		CPPUNIT_ASSERT(!"Should not be here.");
	}
}

void callc_test_class::return_error_3(){
	try{
		INVOKE_THROW_RETURN(int, != 0, posix_error, return_error_22, 1, 2);
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == 2);
	}
	try{
		INVOKE_THROW_RETURN(int, != 0, posix_error, return_no_error_2, 1, 1);
	}catch(...){
		CPPUNIT_ASSERT(!"Should not be here.");
	}
}

static int set_errno(int c) {
	errno = c;
	return -1;
}

static int set_no_errno(int) {
	errno = 0;
	return 0;
}

static int set_errno_21(int c, int) {
	errno = c;
	return -1;
}

static int set_no_errno_2(int, int) {
	errno = 0;
	return 0;
}

static int set_errno_22(int, int c) {
	errno = c;
	return -1;
}

void callc_test_class::set_errno_1() {
	try{
		VOID_INVOKE_THROW(!=0, posix_error, set_errno, 1);
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == 1);
	}
	try{
		VOID_INVOKE_THROW(!=0, posix_error, set_no_errno, 1);
	}catch(...){
		CPPUNIT_ASSERT(!"Should not be here.");
	}
}

void callc_test_class::set_errno_2() {
	try{
		VOID_INVOKE_THROW(!=0, posix_error, set_errno_21, 1, 2);
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == 1);
	}
	try{
		VOID_INVOKE_THROW(!=0, posix_error, set_no_errno_2, 1, 1);
	}catch(...){
		CPPUNIT_ASSERT(!"Should not be here.");
	}
}

void callc_test_class::set_errno_3() {
	try{
		VOID_INVOKE_THROW(!=0, posix_error, set_errno_22, 1, 2);
		CPPUNIT_ASSERT(!"Should not be here.");
	}catch(const posix_error *e){
		CPPUNIT_ASSERT(e->code() == 2);
	}
	try{
		VOID_INVOKE_THROW(!=0, posix_error, set_no_errno_2, 1, 1);
	}catch(...){
		CPPUNIT_ASSERT(!"Should not be here.");
	}
}
