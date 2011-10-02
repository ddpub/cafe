
SET(C_FILES  
  ${CMAKE_CURRENT_SOURCE_DIR}/pos.cpp
  $ENV{CAFE_SRC}/shared/ksi_util/log.cpp
  )
SET(H_FILES
  $ENV{CAFE_SRC}/shared/oracle_client/ksi_exception.hpp
  $ENV{CAFE_SRC}/shared/oracle_client/ksi_common.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pos.hpp
  $ENV{CAFE_SRC}/shared/ksi_util/log.hpp
  )
  
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt $ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)

IF (SOURCE_FILES)
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
	RETURN()
ENDIF (SOURCE_FILES)


PROJECT(HpsClientLibrary)
cmake_minimum_required(VERSION 2.6)

INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD OFF)
FIND_PACKAGE (Boost COMPONENTS system regex date_time)

FIND_LIBRARY (TINYXML_LIB NAMES tinyxml-s PATHS $ENV{CAFE_SRC}/shared/tinyxml)
IF (NOT TINYXML_LIB)
	MESSAGE (FATAL_ERROR "tinyxml lib not found")
ENDIF (NOT TINYXML_LIB)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "system lib found: ${Boost_SYSTEM_LIBRARY}")
MESSAGE (STATUS "regex lib found: ${Boost_REGEX_LIBRARY}")

INCLUDE_DIRECTORIES (..\\ $ENV{INCLUDE} ${Boost_INCLUDE_DIR})
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})


IF (MINGW)
	find_library(EAY32_LIBRARY NAMES eay32 PATHS $ENV{LIB})
	MESSAGE(STATUS "Found eay32 library ${EAY32_LIBRARY}")
	find_library(SSL32_LIBRARY NAMES ssl32 PATHS $ENV{LIB})
	MESSAGE(STATUS "Found ssl32 library ${SSL32_LIBRARY}")
	link_libraries(wsock32 ws2_32 ${SSL32_LIBRARY} ${EAY32_LIBRARY} ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
  ADD_DEFINITIONS(-DTIXML_USE_STL)
ENDIF (MINGW)


IF (UNIX)
find_library(SSL_LIBRARY NAMES ssl PATHS $ENV{LIB})
	MESSAGE(STATUS "Found ssl library ${SSL_LIBRARY}")
ADD_DEFINITIONS(-DTIXML_USE_STL)
ENDIF (UNIX)

#add_library(pos SHARED ${C_FILES})# ${H_FILES})
add_library(pos-s ${C_FILES})

#target_link_libraries(pos ${TINYXML_LIB})

if(MINGW)
target_link_libraries(pos-s ${TINYXML_LIB} ${Boost_DATE_TIME_LIBRARY})
endif(MINGW)

IF (BUILD_TEST)
  add_executable(test test.cc pos.cpp
    /usr/include/tinyxml/tinystr.cpp /usr/include/tinyxml/tinyxml.cpp
    /usr/include/tinyxml/tinyxmlerror.cpp
    /usr/include/tinyxml/tinyxmlparser.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/log.cpp)
  target_link_libraries(test ${SSL_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY} pthread)
ENDIF (BUILD_TEST)

