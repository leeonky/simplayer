#include <string>
#include "ccmedia_test/cuesheet_test.h"
#include "ccmedia/cuesheet.h"
#include "cclibc/exception.h"

using namespace ccmedia;

#define TMP_CUE_FILE "/tmp/test.cue"
void cuesheet_test_class::parse_no_PERFORMER(){
	system("echo '' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue1(TRACE_PARAMS(TMP_CUE_FILE));
	cuesheet cue2(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(cue1.performer(), cue2.performer());
	CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(cue1.performer()));
}

void cuesheet_test_class::parse_empty_PERFORMER(){
	system("echo '' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue1(TRACE_PARAMS(TMP_CUE_FILE));
	system("echo 'PERFORMER \"\"' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(cue.performer()));
	CPPUNIT_ASSERT(cue.performer() != cue1.performer());
}

void cuesheet_test_class::parse_PERFORMER(){
	system("echo 'PERFORMER \"Tom\"' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(std::string("Tom"), std::string(cue.performer()));
}

void cuesheet_test_class::parse_no_TITLE(){
	system("echo '' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue1(TRACE_PARAMS(TMP_CUE_FILE));
	cuesheet cue2(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(cue1.title(), cue2.title());
	CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(cue1.title()));
}

void cuesheet_test_class::parse_empty_TITLE(){
	system("echo '' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue1(TRACE_PARAMS(TMP_CUE_FILE));
	system("echo 'TITLE \"\"' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(cue.title()));
	CPPUNIT_ASSERT(cue.title() != cue1.title());
}

void cuesheet_test_class::parse_TITLE(){
	system("echo 'TITLE \"song\"' > "TMP_CUE_FILE);
	system("echo 'FILE \"CDImage.ape\" WAVE' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(std::string("song"), std::string(cue.title()));
}

void cuesheet_test_class::parse_FILE(){
	system("echo 'FILE \"CDImage.ape\" WAVE' > "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(std::string("CDImage.ape"), std::string(cue.file()));
	CPPUNIT_ASSERT_EQUAL(std::string("/tmp/CDImage.ape"), std::string(cue.full_path()));
}

void cuesheet_test_class::parse_FILE_EXCEPTION(){
	system("echo '' > "TMP_CUE_FILE);
	CPPUNIT_ASSERT_THROW(cuesheet(TRACE_PARAMS(TMP_CUE_FILE)), CCLIBC::exception*);
	system("echo '' > "TMP_CUE_FILE);
	system("echo 'FILE \"\" WAVE' >> "TMP_CUE_FILE);
	CPPUNIT_ASSERT_THROW(cuesheet(TRACE_PARAMS(TMP_CUE_FILE)), CCLIBC::exception*);
}

void cuesheet_test_class::parse_TRACK1(){
	system("echo 'FILE \"CDImage.ape\" WAVE' > "TMP_CUE_FILE);
	system("echo '  TRACK 01 AUDIO' >> "TMP_CUE_FILE);
	system("echo '    INDEX 01 01:01:01' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)1, cue.tracks().size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, cue.tracks().first()->index());
	CPPUNIT_ASSERT_EQUAL((size_t)61013, cue.tracks().first()->start());
	CPPUNIT_ASSERT_EQUAL((size_t)61013, cue.tracks().first()->audio_start());
	CPPUNIT_ASSERT_EQUAL((size_t)0, cue.tracks().first()->end());
	CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(cue.tracks().first()->performer()));
	CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(cue.tracks().first()->title()));
}

void cuesheet_test_class::parse_TRACK1_PERFORMER_TITLE(){
	system("echo 'FILE \"CDImage.ape\" WAVE' > "TMP_CUE_FILE);
	system("echo '  TRACK 01 AUDIO' >> "TMP_CUE_FILE);
	system("echo '    TITLE \"track1\"' >> "TMP_CUE_FILE);
	system("echo '    PERFORMER \"performer1\"' >> "TMP_CUE_FILE);
	system("echo '    INDEX 01 00:00:00' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL(std::string("performer1"), std::string(cue.tracks().first()->performer()));
	CPPUNIT_ASSERT_EQUAL(std::string("track1"), std::string(cue.tracks().first()->title()));
}

void cuesheet_test_class::parse_TRACK1_TRACK2(){
	system("echo 'FILE \"CDImage.ape\" WAVE' > "TMP_CUE_FILE);
	system("echo '  TRACK 01 AUDIO' >> "TMP_CUE_FILE);
	system("echo '    TITLE \"track1\"' >> "TMP_CUE_FILE);
	system("echo '    PERFORMER \"performer1\"' >> "TMP_CUE_FILE);
	system("echo '    INDEX 01 00:00:00' >> "TMP_CUE_FILE);
	system("echo '  TRACK 02 AUDIO' >> "TMP_CUE_FILE);
	system("echo '    TITLE \"track2\"' >> "TMP_CUE_FILE);
	system("echo '    PERFORMER \"performer2\"' >> "TMP_CUE_FILE);
	system("echo '    INDEX 00 00:10:00' >> "TMP_CUE_FILE);
	system("echo '    INDEX 01 00:12:00' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)2, cue.tracks().size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, cue.tracks().first()->index());
	CPPUNIT_ASSERT_EQUAL((size_t)0, cue.tracks().first()->start());
	CPPUNIT_ASSERT_EQUAL((size_t)0, cue.tracks().first()->audio_start());
	CPPUNIT_ASSERT_EQUAL((size_t)10000, cue.tracks().first()->end());
	CPPUNIT_ASSERT_EQUAL(std::string("performer1"), std::string(cue.tracks().first()->performer()));
	CPPUNIT_ASSERT_EQUAL(std::string("track1"), std::string(cue.tracks().first()->title()));
	
	CPPUNIT_ASSERT_EQUAL((size_t)2, cue.tracks().last()->index());
	CPPUNIT_ASSERT_EQUAL((size_t)10000, cue.tracks().last()->start());
	CPPUNIT_ASSERT_EQUAL((size_t)12000, cue.tracks().last()->audio_start());
	CPPUNIT_ASSERT_EQUAL((size_t)0, cue.tracks().last()->end());
	CPPUNIT_ASSERT_EQUAL(std::string("performer2"), std::string(cue.tracks().last()->performer()));
	CPPUNIT_ASSERT_EQUAL(std::string("track2"), std::string(cue.tracks().last()->title()));
}

void cuesheet_test_class::parse_TRACK1_TRACK2_only_INDEX01(){
	system("echo 'FILE \"CDImage.ape\" WAVE' > "TMP_CUE_FILE);
	system("echo '  TRACK 01 AUDIO' >> "TMP_CUE_FILE);
	system("echo '    TITLE \"track1\"' >> "TMP_CUE_FILE);
	system("echo '    PERFORMER \"performer1\"' >> "TMP_CUE_FILE);
	system("echo '    INDEX 01 00:00:00' >> "TMP_CUE_FILE);
	system("echo '  TRACK 02 AUDIO' >> "TMP_CUE_FILE);
	system("echo '    TITLE \"track2\"' >> "TMP_CUE_FILE);
	system("echo '    PERFORMER \"performer2\"' >> "TMP_CUE_FILE);
	system("echo '    INDEX 01 00:12:00' >> "TMP_CUE_FILE);
	cuesheet cue(TRACE_PARAMS(TMP_CUE_FILE));
	CPPUNIT_ASSERT_EQUAL((size_t)12000, cue.tracks().first()->end());
	CPPUNIT_ASSERT_EQUAL((size_t)12000, cue.tracks().last()->start());
	CPPUNIT_ASSERT_EQUAL((size_t)12000, cue.tracks().last()->audio_start());
}
