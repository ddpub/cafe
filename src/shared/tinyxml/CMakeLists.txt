
SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tinystr.cpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml.cpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlerror.cpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlparser.cpp)
SET (H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tinystr.h 
						 ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml.h)
SET (OTHER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

IF (SOURCE_FILES)
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)
	RETURN()
ENDIF (SOURCE_FILES)


PROJECT (TinyXmlLibrary)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_DEFINITIONS(-DTIXML_USE_STL)

#ADD_LIBRARY (tinyxml SHARED ${C_FILES})
ADD_LIBRARY (tinyxml-s STATIC ${C_FILES})

