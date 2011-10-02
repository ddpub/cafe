
SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/printer.cpp 
     ${CMAKE_CURRENT_SOURCE_DIR}/xml_check_printer.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/logo_maker.cpp)
SET (H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/printer.hpp 
     ${CMAKE_CURRENT_SOURCE_DIR}/xml_check_printer.hpp 
     ${CMAKE_CURRENT_SOURCE_DIR}/logo_maker.hpp
     $ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp 
     $ENV{CAFE_SRC}/shared/serial/com.hpp)
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt 
	$ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake 
	$ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)

IF (SOURCE_FILES)
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})        
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT (XmlPrinterLibrary)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)	

IF(WIN32)
	ADD_DEFINITIONS (-D_WIN32_BUILD_)
ENDIF (WIN32)

IF (UNIX)
	ADD_DEFINITIONS (-D_UNIX_BUILD_)
ENDIF (UNIX)

FIND_LIBRARY (SERIAL_LIB NAMES serial-s PATHS $ENV{CAFE_SRC}/shared/serial)

IF (NOT SERIAL_LIB)
	MESSAGE (FATAL_ERROR "serial lib not found")
ENDIF (NOT SERIAL_LIB)

MESSAGE (STATUS "serial lib found: ${SERIAL_LIB}")


FIND_LIBRARY (TINYXML_LIB NAMES tinyxml-s PATHS $ENV{CAFE_SRC}/shared/tinyxml)

IF (NOT TINYXML_LIB)
	MESSAGE (FATAL_ERROR "tinyxml lib not found")
ENDIF (NOT TINYXML_LIB)

MESSAGE (STATUS "tinyxml lib found: ${TINYXML_LIB}")

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD OFF)
FIND_PACKAGE (Boost COMPONENTS system thread)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "system lib found: ${Boost_SYSTEM_LIBRARY}")
MESSAGE (STATUS "thread lib found: ${Boost_THREAD_LIBRARY}")

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR} $ENV{CAFE_SRC}/shared)

IF (WIN32)
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
LINK_LIBRARIES (ws2_32 ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${SERIAL_LIB} ${TINYXML_LIB})
ENDIF (WIN32)

IF (UNIX)
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
LINK_LIBRARIES (${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${SERIAL_LIB} ${TINYXML_LIB})
ENDIF (UNIX)

SET (CMAKE_SHARED_LINKER_FLAGS_RELEASE " -Wl,--allow-multiple-definition -Wl,--enable-auto-import ")
#add_library(printer SHARED ${C_FILES})
#target_link_libraries(printer ${SERIAL_LIB} ${TINYXML_LIB} ${Boost_SYSTEM_LIBRARY})
add_library(printer-s ${C_FILES})
