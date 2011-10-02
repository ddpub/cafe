SET (C_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/treelistctrl.cpp)
SET (H_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/treelistctrl.h)
SET (OTHER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
	
IF (SOURCE_FILES)	
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)	
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/miracle.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/order.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/treelistctrl.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libpos.cmake)
	RETURN()
ENDIF (SOURCE_FILES)


PROJECT (TreeListCtrlLibrary)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (UNIX)
	SET (wxWidgets_USE_LIBS base)
	FIND_PACKAGE (wxWidgets)
	IF (NOT wxWidgets_FOUND)
		MESSAGE (FATAL_ERROR "wxWidgets not found")
	ENDIF (NOT wxWidgets_FOUND)
	INCLUDE (${wxWidgets_USE_FILE})
	INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
ENDIF (UNIX)

IF (MINGW)
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
ENDIF (MINGW)

IF (MINGW)
	INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIR} $ENV{INCLUDE} $ENV{CAFE_SRC}/shared)
	ADD_DEFINITIONS (${WXWIDGETS_DEFINITIONS})

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${WXWIDGETS_CXX_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${WXWIDGETS_EXE_LINKER_FLAGS}")
ENDIF (MINGW)

SET (C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/treelistctrl.cpp)
SET (H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/treelistctrl.h)

IF (MINGW)
MESSAGE (STATUS "${WXWIDGETS_LIBRARIES}")
ENDIF (MINGW)

SET (CMAKE_SHARED_LINKER_FLAGS_RELEASE " -Wl,--allow-multiple-definition -Wl,--enable-auto-import ")
#ADD_LIBRARY (treelistctrl SHARED ${C_FILES})
#target_link_libraries(treelistctrl ${WXWIDGETS_LIBRARIES})
ADD_LIBRARY (treelistctrl-s STATIC ${C_FILES})



