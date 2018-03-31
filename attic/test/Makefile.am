## Process this file with automake to produce Makefile.in
## vi: set noexpandtab:
 
check_PROGRAMS = smile-test
smile_test_SOURCES = \
	AS400ImplTest.cpp \
	CommandTest.cpp \
	ConnectionTest.cpp \
	IfsDirectoryTest.cpp \
	IfsFileHandleTest.cpp \
	IfsFileTest.cpp \
	IfsObjectNameTest.cpp \
	MessageFileTest.cpp \
	NotifierTest.cpp \
	OS400VersionTest.cpp \
	PacketTest.cpp \
	ProgramTest.cpp \
	Properties.cpp \
	PropertiesTest.cpp \
	ServerTest.cpp \
	ServicePoolTest.cpp \
	ServiceProgramProcedureTest.cpp \
	TestEnvironment.cpp \
	TestRunner.cpp \
	TextConverterTest.cpp \
	TextTest.cpp
smile_test_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/tools
smile_test_LDADD = $(top_builddir)/libsmile.la $(top_builddir)/tools/hominid/libhominid.la $(TEST_LIBS)
TESTS = smile-test
