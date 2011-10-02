
IF (SOURCE_FILES)
	execute_process(COMMAND cmake -DSOURCE_FILES=1 -G${CMAKE_GENERATOR} WORKING_DIRECTORY $ENV{CAFE_SRC}/shared/wx_report/reportgen)		
	RETURN()
ELSE (SOURCE_FILES)
ENDIF (SOURCE_FILES)

FIND_LIBRARY (REPORT_LIB NAMES wxreport-s PATHS $ENV{CAFE_SRC}/shared/wx_report/reportgen)

IF (NOT REPORT_LIB)
	MESSAGE (STATUS "try_compile wxreport lib")
	TRY_COMPILE (BUILD_REPORT $ENV{CAFE_SRC}/shared/wx_report/reportgen $ENV{CAFE_SRC}/shared/wx_report/reportgen wxReportLibrary CMAKE_FLAGS -DSOURCE_FILES:UNINITIALIZED=0)

	IF (NOT BUILD_REPORT)
		MESSAGE (FATAL_ERROR "can't build report lib")
	ELSE (NOT BUILD_REPORT)
		MESSAGE (STATUS "compile report lib successful")
	ENDIF (NOT BUILD_REPORT)
ENDIF (NOT REPORT_LIB)
