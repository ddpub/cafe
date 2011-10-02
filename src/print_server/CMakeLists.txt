
SET (C_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/xml_config.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/version.cpp)
SET (H_FILES 
	$ENV{CAFE_SRC}/shared/ksi_util/xml_config.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/logger.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/algo_functor.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/monitor.hpp)
SET (OTHER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt 
	$ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)

IF (SOURCE_FILES)	
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)	
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT (PrintServerApp)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)


IF (WIN32)
	ADD_DEFINITIONS(-D_WIN32_BUILD_)
ENDIF (WIN32)

IF (UNIX)
	ADD_DEFINITIONS(-D_UNIX_BUILD_)
ENDIF (UNIX)

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD OFF)
FIND_PACKAGE (Boost COMPONENTS date_time thread system)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "date_time lib found: ${Boost_DATE_TIME_LIBRARY}")
MESSAGE (STATUS "thread lib found: ${Boost_THREAD_LIBRARY}")
MESSAGE (STATUS "system lib found: ${Boost_SYSTEM_LIBRARY}")

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR} $ENV{CAFE_SRC}/shared)

FIND_LIBRARY (TINYXML_LIB NAMES tinyxml-s PATHS $ENV{CAFE_SRC}/shared/tinyxml)

IF (NOT TINYXML_LIB)
	MESSAGE (FATAL_ERROR "tinyxml lib not found")
ENDIF (NOT TINYXML_LIB)

FIND_LIBRARY (PRINTER_LIB NAMES printer-s PATHS $ENV{CAFE_SRC}/shared/printer)

IF (NOT PRINTER_LIB)
	MESSAGE (FATAL_ERROR "printer lib not found")
ENDIF (NOT PRINTER_LIB)

FIND_LIBRARY (SERIAL_LIB NAMES serial-s PATHS $ENV{CAFE_SRC}/shared/serial)

IF (NOT SERIAL_LIB)
	MESSAGE (FATAL_ERROR "serial lib not found")
ENDIF (NOT SERIAL_LIB)

IF (MINGW)
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
	LINK_LIBRARIES (wsock32 ws2_32 ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB})
ENDIF (MINGW)

IF (UNIX)
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
	LINK_LIBRARIES (${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB})
ENDIF (UNIX)

ADD_EXECUTABLE (print_server WIN32 ${C_FILES})

IF (MINGW)
	TARGET_LINK_LIBRARIES(print_server  wsock32 ws2_32 ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB})
ENDIF (MINGW)