
SET (C_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sync.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/kitchen_util.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/kitchen_receipt.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/connect.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/xml_config.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/logger.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/users.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/util.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/date_time.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/version.cpp)
SET (H_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/sync.hpp
	$ENV{CAFE_SRC}/shared/shared_modules/kitchen_util.hpp
	$ENV{CAFE_SRC}/shared/shared_modules/kitchen_receipt.hpp
	$ENV{CAFE_SRC}/shared/shared_modules/connect.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/xml_config.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/logger.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/algo_functor.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/users.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/date_time.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/util.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/thread_pulse.hpp)
SET (OTHER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt 
	$ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)

IF (SOURCE_FILES)	
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)	
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT (KitchenInformerApp)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)

FIND_LIBRARY (TINYXML_LIB NAMES tinyxml-s PATHS $ENV{CAFE_SRC}/shared/tinyxml)
IF (NOT TINYXML_LIB)
	MESSAGE (FATAL_ERROR "tinyxml lib not found")
ENDIF (NOT TINYXML_LIB)
MESSAGE (STATUS "tinyxml lib found: ${TINYXML_LIB}")

FIND_LIBRARY (SERIAL_LIB NAMES serial-s PATHS $ENV{CAFE_SRC}/shared/serial)
IF (NOT SERIAL_LIB)
	MESSAGE (FATAL_ERROR "serial lib not found")
ENDIF (NOT SERIAL_LIB)
MESSAGE (STATUS "serial lib found: ${SERIAL_LIB}")

FIND_LIBRARY (PRINTER_LIB NAMES printer-s PATHS $ENV{CAFE_SRC}/shared/printer)
IF (NOT PRINTER_LIB)
	MESSAGE (FATAL_ERROR "printer lib not found")
ENDIF (NOT PRINTER_LIB)
MESSAGE (STATUS "printer lib found: ${PRINTER_LIB}")

FIND_LIBRARY(ORACLE_LIB_CLNTSH NAMES clntsh oci.lib PATHS $ENV{ORACLE_HOME} $ENV{ORACLE_HOME}/lib $ENV{ORACLE_HOME}/oci/lib/msvc/)
IF (NOT ORACLE_LIB_CLNTSH)
	MESSAGE(FATAL_ERROR "oracle client library not found")
ENDIF (NOT ORACLE_LIB_CLNTSH)
MESSAGE(STATUS "oracle client lib found: ${ORACLE_LIB_CLNTSH}")

FIND_LIBRARY (ORACLE_LIB NAMES oracle-s PATHS $ENV{CAFE_SRC}/shared/ksi_libdb)
IF (NOT ORACLE_LIB)
	MESSAGE (FATAL_ERROR "oracle lib not found")
ENDIF (NOT ORACLE_LIB)
MESSAGE(STATUS "oracle found: ${ORACLE_LIB}")


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

IF (MINGW)
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
	LINK_LIBRARIES (wsock32 ws2_32 ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB} ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB})
ENDIF (MINGW)

IF (UNIX)
	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
	LINK_LIBRARIES (${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB} ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB})
ENDIF (UNIX)

ADD_EXECUTABLE (kitchen_informer WIN32 ${C_FILES})

IF (MINGW)
	TARGET_LINK_LIBRARIES(kitchen_informer  wsock32 ws2_32 ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB} ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB})
ENDIF (MINGW)

IF (UNIX)
	TARGET_LINK_LIBRARIES(kitchen_informer ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${TINYXML_LIB} ${PRINTER_LIB} ${SERIAL_LIB} ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB})
ENDIF (UNIX)

IF (UNIX)
	SET (BIN_DIR /usr/local/ksi/bin/)
	INSTALL (CODE "MESSAGE (\"Installing binaries to ${BIN_DIR} from ${CMAKE_CURRENT_BINARY_DIR} ...\")")
	EXECUTE_PROCESS (COMMAND date +%Y%m%d%H%M%S OUTPUT_VARIABLE DATE RESULT_VARIABLE RET_VAL)
	STRING (SUBSTRING ${DATE} 0 14 TRIM_DATE)
	INSTALL (CODE "EXECUTE_PROCESS (COMMAND mv ${BIN_DIR}/kitchen_informer ${BIN_DIR}/kitchen_informer.${TRIM_DATE})")
	INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/kitchen_informer DESTINATION ${BIN_DIR}/ PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_READ WORLD_WRITE WORLD_EXECUTE)
	INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/kitchen_informer.sh DESTINATION ${BIN_DIR}/ PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_READ WORLD_WRITE WORLD_EXECUTE)
	#INSTALL (CODE "EXECUTE_PROCESS (COMMAND chmod 777 ${BIN_DIR}/kitchen_informer)")
	INSTALL (CODE "MESSAGE (\"installed.\")")
ENDIF (UNIX)