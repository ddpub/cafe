SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/dbsync.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MemoOutPut.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sync.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp
  $ENV{CAFE_SRC}/shared/ksi_util/bag_spooler.cpp 
  $ENV{CAFE_SRC}/shared/ksi_util/url.cpp
  $ENV{CAFE_SRC}/shared/ksi_util/log.cpp)
SET (H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/MemoOutPut.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sync.h
  $ENV{CAFE_SRC}/shared/ksi_util/bag_spooler.hpp 
  $ENV{CAFE_SRC}/shared/ksi_util/url.hpp
  $ENV{CAFE_SRC}/shared/ksi_util/log.hpp
  $ENV{CAFE_SRC}/shared/ksi_include/monitor.hpp
  $ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp)
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
  $ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)

IF (SOURCE_FILES)	
  foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
    MESSAGE (STATUS "!@# ${file}")
  endforeach(file)	
  INCLUDE($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)
  RETURN()
ENDIF (SOURCE_FILES)


PROJECT (DBSync)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)

find_library(ORACLE_LIB_CLNTSH NAMES clntsh oci.lib PATHS $ENV{ORACLE_HOME}
  $ENV{ORACLE_HOME}/lib $ENV{ORACLE_HOME}/oci/lib/msvc/)
if (NOT ORACLE_LIB_CLNTSH)
  MESSAGE(FATAL_ERROR "oracle client library not found")
endif (NOT ORACLE_LIB_CLNTSH)

MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB_CLNTSH}")

find_library(ORACLE_LIB NAMES oracle-s PATHS $ENV{CAFE_SRC}/shared/ksi_libdb)
IF (NOT ORACLE_LIB)
  MESSAGE (FATAL_ERROR "oracle lib not found")
ENDIF (NOT ORACLE_LIB)
MESSAGE(STATUS "Ksi libdb found: ${ORACLE_LIB}")

SET (BOOST_USE_STATIC_LIBS ON)
SET (BOOST_USE_MULTITHREADED OFF)

FIND_PACKAGE( Boost COMPONENTS thread date_time system)

IF (NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "thread lib found: ${Boost_THREAD_LIBRARY}")

INCLUDE_DIRECTORIES ($ENV{CAFE_SRC}/shared/oracle_client  $ENV{CAFE_SRC}/shared/ksi_libdb /usr/include/python2.5/ ${Boost_INCLUDE_DIR} ..\\oracle_client $ENV{POSTGRES_HOME}/include $ENV{CAFE_SRC}/shared) 
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS} ${ORACLE_LIB} $ENV{CAFE_SRC}/shared/oracle_client $ENV{CAFE_SRC}/shared/ksi_libdb)

ADD_EXECUTABLE (dbsync  ${C_FILES})

IF (WIN32)
  target_link_libraries(dbsync wsock32 ws2_32 ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${ORACLE_LIB} ${LIBPQ} ${ORACLE_LIB_CLNTSH})
ELSE (WIN32)
  target_link_libraries(dbsync ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${ORACLE_LIB}
    ${ORACLE_LIB_CLNTSH})
ENDIF (WIN32)






