
SET (C_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/cafe_container.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/cctrans_manager.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/coupon.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/credit_card.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/menu_container.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/menu_interface.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/schedule_container.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/order_interface.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/order_payment.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/delivery_info.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/connect.cpp 
	$ENV{CAFE_SRC}/shared/ksi_util/util.cpp)
SET (H_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/cafe_container.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/cctrans_manager.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/coupon.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/credit_card.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/menu_container.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/menu_interface.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/schedule_container.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/order_interface.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/order_payment.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/delivery_info.hpp
	$ENV{CAFE_SRC}/shared/shared_modules/connect.hpp 
	$ENV{CAFE_SRC}/shared/ksi_util/util.hpp 
	$ENV{CAFE_SRC}/shared/oracle_client/ksi_exception.hpp 
	$ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp 
	$ENV{CAFE_SRC}/shared/ksi_include/algo_functor.hpp)
SET (OTHER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

IF (SOURCE_FILES)
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)
	RETURN()
ENDIF (SOURCE_FILES)

PROJECT (OrderLibrary)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD OFF)
FIND_PACKAGE (Boost COMPONENTS system)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

MESSAGE (STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
MESSAGE (STATUS "system lib found: ${Boost_SYSTEM_LIBRARY}")
MESSAGE (STATUS $ENV{CAFE_SRC}/shared)

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR} $ENV{CAFE_SRC}/shared)
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})

#ADD_LIBRARY (order SHARED ${C_FILES})
ADD_LIBRARY (order-s STATIC ${C_FILES})

