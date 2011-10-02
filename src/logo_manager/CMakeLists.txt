
SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lm_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/base_lm_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/options_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/base_options_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/lmApp.cpp
             $ENV{CAFE_SRC}/shared/wx_util/wx_util.cpp
             $ENV{CAFE_SRC}/shared/xml_config.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp)
SET (H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lm_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/base_lm_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/options_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/base_options_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/lmApp.h
             $ENV{CAFE_SRC}/shared/wx_util/wx_util.hpp
             $ENV{CAFE_SRC}/shared/xml_config.hpp
             $ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp
             $ENV{CAFE_SRC}/shared/ksi_include/algo_functor.hpp
             $ENV{CAFE_SRC}/shared/ksi_util/util.hpp)
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
                 $ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake
                 $ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)

IF (SOURCE_FILES)	
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)	
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT (LogoManagerApp)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

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
FIND_PACKAGE (Boost COMPONENTS system thread)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "system lib found: ${Boost_SYSTEM_LIBRARY}")
MESSAGE (STATUS "thread lib found: ${Boost_THREAD_LIBRARY}")


FIND_LIBRARY (PRINTER_LIB NAMES printer-s PATHS $ENV{CAFE_SRC}/shared/printer)

IF (NOT PRINTER_LIB)
	MESSAGE (FATAL_ERROR "printer lib not found")
ENDIF (NOT PRINTER_LIB)

FIND_LIBRARY (SERIAL_LIB NAMES serial-s PATHS $ENV{CAFE_SRC}/shared/serial)

IF (NOT SERIAL_LIB)
	MESSAGE (FATAL_ERROR "serial lib not found")
ENDIF (NOT SERIAL_LIB)


FIND_LIBRARY (TINYXML_LIB NAMES tinyxml-s PATHS $ENV{CAFE_SRC}/shared/tinyxml)

IF (NOT TINYXML_LIB)
	MESSAGE (FATAL_ERROR "tinyxml lib not found")
ENDIF (NOT TINYXML_LIB)

MESSAGE (STATUS "tinyxml lib found: ${TINYXML_LIB}")

IF (WIN32)

	SET (WXW_USE_DEBUG ON)
	SET (WXW_USE_SHARED OFF)
	SET (WXW_USE_UNICODE ON)
	SET (WXW_USE_UNIV OFF)
	SET (WXW_USE_MONO OFF)
	SET (WXWIDGETS_FILE_VERSION "28")
	SET (WXWIDGETS_VERSION "2.8")

	INCLUDE (FindwxW)

	IF (NOT WXWIDGETS_FOUND)
		MESSAGE(FATAL_ERROR "wxWidgets not found")
	ENDIF (NOT WXWIDGETS_FOUND)

	INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIR} ${Boost_INCLUDE_DIR} $ENV{CAFE_SRC}/shared)
	ADD_DEFINITIONS (${WXWIDGETS_DEFINITIONS})

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${WXWIDGETS_CXX_FLAGS} -static")

	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${WXWIDGETS_EXE_LINKER_FLAGS}")

	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS} ${WXWIDGETS_LINK_DIRECTORIES})
	LINK_LIBRARIES ( ws2_32 ${WXWIDGETS_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${SERIAL_LIB} ${PRINTER_LIB} ${TINYXML_LIB})

	ADD_EXECUTABLE (logo_manager WIN32 ${C_FILES})
	target_link_libraries(logo_manager ws2_32 ${WXWIDGETS_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${SERIAL_LIB} ${PRINTER_LIB} ${TINYXML_LIB})
ENDIF (WIN32)
	
IF (UNIX)
	SET (wxWidgets_USE_LIBS base core xml)
	FIND_PACKAGE (wxWidgets)
	IF (NOT wxWidgets_FOUND)
		MESSAGE (STATUS "wxWidgets not found")
	ENDIF (NOT wxWidgets_FOUND)
	MESSAGE (STATUS ">>>>${CMAKE_CXX_FLAGS}")
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${wxWidgets_CXX_FLAGS}")
	MESSAGE (STATUS ">>>>${CMAKE_CXX_FLAGS}")
	INCLUDE (${wxWidgets_USE_FILE})
	INCLUDE_DIRECTORIES (${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} $ENV{CAFE_SRC}/shared)
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
	ADD_EXECUTABLE (logo_manager ${C_FILES})
	TARGET_LINK_LIBRARIES(logo_manager ${wxWidgets_LIBRARIES} ${SERIAL_LIB} ${PRINTER_LIB} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${TINYXML_LIB})
ENDIF (UNIX)
