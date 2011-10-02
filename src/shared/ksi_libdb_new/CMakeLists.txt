
SET(C_FILES 
						${CMAKE_CURRENT_SOURCE_DIR}/datatypes.cc
						${CMAKE_CURRENT_SOURCE_DIR}/oci_adapter.cc
						${CMAKE_CURRENT_SOURCE_DIR}/oci_op.cc
						${CMAKE_CURRENT_SOURCE_DIR}/oracle.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/oci_adapter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/oci_lob.cc
            )
SET(H_FILES
						${CMAKE_CURRENT_SOURCE_DIR}/bind_datatypes.hh
						${CMAKE_CURRENT_SOURCE_DIR}/database_exception.hpp
						${CMAKE_CURRENT_SOURCE_DIR}/datatypes.hh
						${CMAKE_CURRENT_SOURCE_DIR}/oci_adapter.hh
						${CMAKE_CURRENT_SOURCE_DIR}/oci_attributes.hh
						${CMAKE_CURRENT_SOURCE_DIR}/oci_handles.hh
						${CMAKE_CURRENT_SOURCE_DIR}/oci_op.hh
						${CMAKE_CURRENT_SOURCE_DIR}/oracle.hh
            )
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

IF (SOURCE_FILES)
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)
	RETURN()
ENDIF (SOURCE_FILES)


PROJECT(OracleClientLibrary)
cmake_minimum_required(VERSION 2.6)


find_library(ORACLE_LIB_CLNTSH NAMES clntsh oci.lib PATHS $ENV{ORACLE_HOME}
  $ENV{ORACLE_HOME}/lib $ENV{ORACLE_HOME}/oci/lib/msvc/)
IF (NOT ORACLE_LIB_CLNTSH)
  MESSAGE(FATAL_ERROR "oracle client library not found")
ENDIF (NOT ORACLE_LIB_CLNTSH)
MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB_CLNTSH}")

IF(UNIX)
  find_library(ORACLE_LIB_OCIEI ociei PATHS $ENV{ORACLE_HOME} $ENV{ORACLE_HOME}/lib)
  IF (NOT ORACLE_LIB_OCIEI)
    MESSAGE(FATAL_ERROR "oracle client library not found")
  ENDIF (NOT ORACLE_LIB_OCIEI)
  MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB_OCIEI}")
ENDIF(UNIX)

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREADED:BOOL OFF)

IF (WITH_PYTHON)
  FIND_PACKAGE( Boost COMPONENTS date_time system python regex)
ELSE (WITH_PYTHON)
  FIND_PACKAGE( Boost COMPONENTS date_time system regex thread)
ENDIF (WITH_PYTHON)
IF (NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ..\\oracle_client)
IF (WIN32)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LINK_LIBRARIES(wsock32 ws2_32 ${Boost_DATETIME_LIBRARY} ${Boost_REGEX_LIBRARY}
  ${Boost_THREAD_LIBRARY})
ENDIF (WIN32)
IF (UNIX)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#LINK_LIBRARIES(${Boost_DATETIME_LIBRARY} ${Boost_REGEX_LIBRARY})
ENDIF (UNIX)

IF (WITH_PYTHON)
find_library(LIB_PYTHON NAMES python2.5 PATHS $ENV{LIB})
IF (NOT LIB_PYTHON)
  MESSAGE(FATAL_ERROR "Python library not found")
ENDIF (NOT LIB_PYTHON)
MESSAGE(STATUS "Python lib found: ${LIB_PYTHON}")
LINK_LIBRARIES(${LIB_PYTHON})
ENDIF (WITH_PYTHON)

IF(UNIX)
  SET(LIBPQ_LIB pq)
ELSE(UNIX)
  SET(LIBPQ_LIB libpq.lib)
  ADD_DEFINITIONS(-D__WIN32__)
ENDIF(UNIX)

#find_library(LIBPQ NAMES ${LIBPQ_LIB} PATHS $ENV{LIB}
#  $ENV{POSTGRES_HOME}/lib)
#IF (NOT LIBPQ)
#  MESSAGE(FATAL_ERROR "PostgresQL library not found")
#ENDIF (NOT LIBPQ)
#MESSAGE(STATUS "Postgres lib found: ${LIBPQ}")

include_directories(..\\common ..\\liboracle_client $ENV{ORACLE_HOME}/include
  $ENV{ORACLE_HOME}/sdk/include
  ../common ../liboracle_client /usr/include/python2.5/
  $ENV{ORACLE_HOME}/oci/include/ $ENV{POSTGRES_HOME}/include)
#link_libraries(${ORACLE_LIB_CLNTSH} ${ORACLE_LIB_OCIEI})

#add_library(oracle SHARED ${C_FILES})
add_library(oracle SHARED ${C_FILES})# ${H_FILES})
IF (WIN32)
target_link_libraries(oracle ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB_OCIEI} ${WSOCK32} ${WS2_32}) 
ENDIF (WIN32)
add_library(oracle-s ${C_FILES})

IF (BUILD_TEST MATCHES TRUE)
  add_executable(test test.cc)
  target_link_libraries(test oracle-s ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB_OCIEI}) 
ENDIF (BUILD_TEST MATCHES TRUE)
