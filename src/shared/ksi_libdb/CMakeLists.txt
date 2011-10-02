IF (NOT COMMON_DRIVER)
  SET(C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/oracle.cc)
ELSE (NOT COMMON_DRIVER)
  SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/oracle_driver_stub.cc ${CMAKE_CURRENT_SOURCE_DIR}/oracle_driver.cpp)
ENDIF (NOT COMMON_DRIVER)
SET(H_FILES $ENV{CAFE_SRC}/shared/oracle_client/ksi_common.hpp $ENV{CAFE_SRC}/shared/oracle_client/ksi_exception.hpp
  $ENV{CAFE_SRC}/shared/oracle_client/oracle_client.hpp)
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
  
IF (SOURCE_FILES)
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT(OracleClientLibrary)
cmake_minimum_required(VERSION 2.6)


find_library(ORACLE_LIB_CLNTSH NAMES clntsh11 oci.lib PATHS $ENV{ORACLE_HOME}
  $ENV{ORACLE_HOME}/lib $ENV{ORACLE_HOME}/oci/lib/msvc $ENV{ORACLE_HOME}/sdk/lib/msvc)
IF (NOT ORACLE_LIB_CLNTSH)
  find_library(ORACLE_LIB_CLNTSH NAMES clntsh PATHS $ENV{ORACLE_HOME}
    $ENV{ORACLE_HOME}/lib)
  if (NOT ORACLE_LIB_CLNTSH)
    MESSAGE(FATAL_ERROR "oracle client library not found")
  endif (NOT ORACLE_LIB_CLNTSH)
ENDIF (NOT ORACLE_LIB_CLNTSH)
MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB_CLNTSH}")


SET (BOOST_USE_STATIC_LIBS ON)
SET (BOOST_USE_MULTITHREADED OFF)

FIND_PACKAGE( Boost COMPONENTS thread)

IF (NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "thread lib found: ${Boost_THREAD_LIBRARY}")


INCLUDE_DIRECTORIES (..\\common ..\\liboracle_client $ENV{ORACLE_HOME} $ENV{ORACLE_HOME}/sdk/include $ENV{ORACLE_HOME}/include $ENV{ORACLE_HOME}/oci/include ../common ../liboracle_client /usr/include/python2.5/ ${Boost_INCLUDE_DIR} ..\\oracle_client $ENV{POSTGRES_HOME}/include $ENV{CAFE_SRC}/shared $ENV{ORACLE_HOME}/rdbms/public)
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})

#ADD_LIBRARY (oracle SHARED ${C_FILES})
ADD_LIBRARY (oracle-s STATIC ${C_FILES})

IF (DONT_USE_THREADS)
   ADD_DEFINITIONS(-D_DONT_USE_THREADS_)
ENDIF(DONT_USE_THREADS)

FIND_LIBRARY(ORACLE_LIB oracle-s PATHS ./)

IF (BUILD_TEST)
  ADD_EXECUTABLE(driver_test test.cc)
  target_link_libraries(driver_test ${ORACLE_LIB_CLNTSH} ${ORACLE_LIB} ${Boost_THREAD_LIBRARY}) 
ENDIF(BUILD_TEST)

IF (WIN32)
	#target_link_libraries(oracle ${ORACLE_LIB_CLNTSH} wsock32 ws2_32 ${Boost_THREAD_LIBRARY}) 
ENDIF (WIN32)



