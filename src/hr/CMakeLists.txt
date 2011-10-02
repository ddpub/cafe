SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/HR_users.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/base_connect.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_position_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/auth_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/wx_authorise.cpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/base_auth_f.cpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/base_text_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/text_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/mcard.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/dialog_f.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/position_f.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_person_f.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_property_f.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_HR_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/person_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/HR_f.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/HR_ico.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/property_f.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/application_register.cpp
			 $ENV{CAFE_SRC}/shared/miracle/convert.cpp
			 $ENV{CAFE_SRC}/shared/serial/com.cpp
			 $ENV{CAFE_SRC}/shared/ksi_util/date_time.cpp
			 $ENV{CAFE_SRC}/shared/lang/xml_lang.cpp 
             $ENV{CAFE_SRC}/shared/connect.cpp 
             $ENV{CAFE_SRC}/shared/ksi_util/users.cpp 
			 $ENV{CAFE_SRC}/shared/ksi_util/bag_spooler.cpp 
			 $ENV{CAFE_SRC}/shared/ksi_util/url.cpp 
			 $ENV{CAFE_SRC}/shared/ksi_util/log.cpp 
             $ENV{CAFE_SRC}/shared/wx_util/wx_util.cpp 
             $ENV{CAFE_SRC}/shared/ksi_util/util.cpp
             $ENV{CAFE_SRC}/shared/wx_util/xml_config.cpp)
SET (H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/HR_users.h
             ${CMAKE_CURRENT_SOURCE_DIR}/auth_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/base_connect.hpp 
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_position_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/base_auth_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/wx_authorise.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/base_text_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/text_f.h  
             ${CMAKE_CURRENT_SOURCE_DIR}/mcard.h 
             ${CMAKE_CURRENT_SOURCE_DIR}/dialog_f.h
			 ${CMAKE_CURRENT_SOURCE_DIR}/position_f.h
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_person_f.h
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_property_f.h
			 ${CMAKE_CURRENT_SOURCE_DIR}/base_HR_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/person_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/HR_f.h
             ${CMAKE_CURRENT_SOURCE_DIR}/main.h
             ${CMAKE_CURRENT_SOURCE_DIR}/HR_ico.h
			 ${CMAKE_CURRENT_SOURCE_DIR}/property_f.h
			 ${CMAKE_CURRENT_SOURCE_DIR}/application_register.hpp
			 $ENV{CAFE_SRC}/shared/miracle/convert.hpp
			 $ENV{CAFE_SRC}/shared/serial/com.hpp
			 $ENV{CAFE_SRC}/shared/ksi_util/date_time.hpp
			 $ENV{CAFE_SRC}/shared/lang/xml_lang.h 
             $ENV{CAFE_SRC}/shared/connect.hpp 
             $ENV{CAFE_SRC}/shared/ksi_util/util.hpp
             $ENV{CAFE_SRC}/shared/ksi_util/users.hpp 
			 $ENV{CAFE_SRC}/shared/ksi_util/bag_spooler.hpp 
			 $ENV{CAFE_SRC}/shared/ksi_util/log.hpp 
			 $ENV{CAFE_SRC}/shared/ksi_include/monitors.hpp 
			 $ENV{CAFE_SRC}/shared/ksi_util/url.hpp 
             $ENV{CAFE_SRC}/shared/wx_util/wx_util.hpp 
             $ENV{CAFE_SRC}/shared/wx_util/xml_config.hpp) 
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
				 ${CMAKE_CURRENT_SOURCE_DIR}/hrmanager.rc
                 $ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake
				 $ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake
				 )

IF (SOURCE_FILES)	
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")	
	endforeach(file)	
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT (HRManagerApp)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD ON)

FIND_PACKAGE (Boost COMPONENTS date_time thread system)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "system lib found: ${Boost_SYSTEM_LIBRARY}")


find_library(LIBPQ NAMES libpq.lib PATHS $ENV{LIB} $ENV{POSTGRES_HOME}/lib)
IF (NOT LIBPQ)
	MESSAGE(FATAL_ERROR "PostgresQL library not found")
ENDIF (NOT LIBPQ)
MESSAGE(STATUS "Postgres lib found: ${LIBPQ}")

find_library(ORACLE_LIB_CLNTSH NAMES clntsh oci.lib PATHS $ENV{ORACLE_HOME}
  $ENV{ORACLE_HOME}/lib $ENV{ORACLE_HOME}/oci/lib/msvc/)
IF (NOT ORACLE_LIB_CLNTSH)
  MESSAGE(FATAL_ERROR "oracle client library not found")
ENDIF (NOT ORACLE_LIB_CLNTSH)
MESSAGE(STATUS "oracle client lib found: ${ORACLE_LIB_CLNTSH}")

find_library(ORACLE_LIB NAMES oracle-s PATHS $ENV{CAFE_SRC}/shared/ksi_libdb)
IF (NOT ORACLE_LIB)
	MESSAGE (FATAL_ERROR "oracle lib not found")
ENDIF (NOT ORACLE_LIB)
MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB}")

FIND_LIBRARY (SERIAL_LIB NAMES serial-s PATHS $ENV{CAFE_SRC}/shared/serial)
IF (NOT SERIAL_LIB)
	MESSAGE (FATAL_ERROR "serial lib not found")
ENDIF (NOT SERIAL_LIB)
MESSAGE(STATUS "serial lib found: ${SERIAL_LIB}")

IF (WIN32)
	
	SET (WXW_USE_DEBUG OFF)
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
	
	IF (MINGW)
	
		#FIND_LIBRARY (WS2_32 NAMES ws2_32.lib ws2_32 PATHS $ENV{LIB})
		#IF (NOT WS2_32)
		#	MESSAGE (FATAL_ERROR "ws2_32 not found")
		#ENDIF (NOT WS2_32) 
		#MESSAGE(STATUS "ws2_32 lib found: ${WS2_32}")

		#FIND_LIBRARY (WSOCK32 NAMES wsock32.lib wsock32 PATHS $ENV{LIB})
		#IF (NOT WSOCK32)
		#	MESSAGE (FATAL_ERROR "wsock32 not fould")
		#ENDIF (NOT WSOCK32)
		#MESSAGE(STATUS "wsock32 lib found: ${WSOCK32}")
	
		ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/hrmanager_rc.o
		COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
		-i ${CMAKE_CURRENT_SOURCE_DIR}/hrmanager.rc
		-o ${CMAKE_CURRENT_BINARY_DIR}/hrmanager_rc.o)
	ENDIF (MINGW)

    ADD_DEFINITIONS (${WXWIDGETS_DEFINITIONS})
	ADD_DEFINITIONS(-D_WIN32_BUILD_)
ENDIF (WIN32)

IF (UNIX)

	SET (wxWidgets_USE_LIBS base core)
	
	FIND_PACKAGE (wxWidgets)
	IF (NOT wxWidgets_FOUND)
		MESSAGE (STATUS "wxWidgets not found")
	ENDIF (NOT wxWidgets_FOUND)
	
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${wxWidgets_CXX_FLAGS}")
	INCLUDE (${wxWidgets_USE_FILE})
	ADD_DEFINITIONS(-D_UNIX_BUILD_)
ENDIF (UNIX)

INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIR} ${Boost_INCLUDE_DIR} $ENV{CAFE_SRC}/shared)
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS} ${WXWIDGETS_LINK_DIRECTORIES})
LINK_LIBRARIES (wsock32 ws2_32 ${Boost_SYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_THREAD_LIBRARY} ${WXWIDGETS_LIBRARIES} ${ORACLE_LIB}  ${ORACLE_LIB_CLNTSH} ${LIBPQ} ) 
ADD_EXECUTABLE (HRManager WIN32 ${C_FILES} ${CMAKE_CURRENT_BINARY_DIR}/hrmanager_rc.o)

IF (WIN32)
	target_link_libraries(HRManager wsock32 ws2_32 ${ORACLE_LIB} ${ORACLE_LIB_CLNTSH} ${LIBPQ} ${WXWIDGETS_LIBRARIES} ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}) 
ENDIF (WIN32)

