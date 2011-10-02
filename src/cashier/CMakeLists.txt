SET (C_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/cashierApp.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/base_cashier_f.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/cashier_f.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/version.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/cashier_ico.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_auth_f.cpp 
    $ENV{CAFE_SRC}/shared/shared_modules/auth_f.cpp 
    $ENV{CAFE_SRC}/shared/shared_modules/wx_authorise.cpp 
    $ENV{CAFE_SRC}/shared/shared_modules/base_add_modifiers_f.cpp 
    $ENV{CAFE_SRC}/shared/shared_modules/add_modifiers_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_btn_list_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/btn_list_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/coupon_list_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_coupon_calc_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/coupon_calc_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_count_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/count_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_calc_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/calc_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_text_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/text_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_card_swipe_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/card_swipe_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_manual_entry_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/manual_entry_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_tip_amount_f.cpp 
	$ENV{CAFE_SRC}/shared/shared_modules/tip_amount_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_close_order_manager_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/close_order_manager_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_input_num_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/input_num_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_config_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/config_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_advance_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/advance_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_delayed_order_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/delayed_order_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/payment_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/receipt.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_select_date_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/select_date_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_select_time_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/select_time_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_load_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/load_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_info_f.cpp
    $ENV{CAFE_SRC}/shared/shared_modules/info_f.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/fin_load_f.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/base_dlg_f.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/dlg_f.cpp
    $ENV{CAFE_SRC}/shared/wx_util/move_up.cpp 
    $ENV{CAFE_SRC}/shared/wx_util/order_show.cpp  
    $ENV{CAFE_SRC}/shared/wx_util/wx_util.cpp 
    $ENV{CAFE_SRC}/shared/wx_util/xml_config.cpp
	$ENV{CAFE_SRC}/shared/wx_util/credit_utils.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/application_register.cpp 
    $ENV{CAFE_SRC}/shared/ksi_util/users.cpp 
    $ENV{CAFE_SRC}/shared/ksi_util/util.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/log.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/reg.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/cash_advance.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/inet.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/date_time.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/format.cpp
    $ENV{CAFE_SRC}/shared/ksi_util/currency.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/loader.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/bag_spooler.cpp
	$ENV{CAFE_SRC}/shared/ksi_util/url.cpp
	$ENV{CAFE_SRC}/shared/drawer/drawer.cpp
	$ENV{CAFE_SRC}/shared/shared_modules/connect.cpp
	$ENV{CAFE_SRC}/shared/lang/xml_lang.cpp)
SET (H_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/cashierApp.h  
    ${CMAKE_CURRENT_SOURCE_DIR}/base_cashier_f.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cashier_f.h
	$ENV{CAFE_SRC}/shared/shared_modules/cashier_ico.h	
    $ENV{CAFE_SRC}/shared/shared_modules/base_auth_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/auth_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/wx_authorise.hpp 
    $ENV{CAFE_SRC}/shared/shared_modules/base_add_modifiers_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/add_modifiers_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_btn_list_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/btn_list_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/coupon_list_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_coupon_calc_f.h
	$ENV{CAFE_SRC}/shared/shared_modules/coupon_calc_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_count_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/count_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_calc_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/calc_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_text_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/text_f.h    
    $ENV{CAFE_SRC}/shared/shared_modules/base_card_swipe_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/card_swipe_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_manual_entry_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/manual_entry_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_tip_amount_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/tip_amount_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_close_order_manager_f.h
	$ENV{CAFE_SRC}/shared/shared_modules/close_order_manager_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_input_num_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/input_num_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_config_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/config_f.h 
    $ENV{CAFE_SRC}/shared/shared_modules/base_advance_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/advance_f.h 
    $ENV{CAFE_SRC}/shared/shared_modules/base_delayed_order_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/delayed_order_f.h        
    $ENV{CAFE_SRC}/shared/shared_modules/payment_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/receipt.hpp
    $ENV{CAFE_SRC}/shared/shared_modules/base_select_date_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/select_date_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_select_time_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/select_time_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_load_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/load_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/base_info_f.h
    $ENV{CAFE_SRC}/shared/shared_modules/info_f.h
	$ENV{CAFE_SRC}/shared/shared_modules/fin_load_f.h
	$ENV{CAFE_SRC}/shared/shared_modules/base_dlg_f.h
	$ENV{CAFE_SRC}/shared/shared_modules/dlg_f.h
    $ENV{CAFE_SRC}/shared/wx_util/move_up.hpp 
    $ENV{CAFE_SRC}/shared/wx_util/order_show.hpp  
    $ENV{CAFE_SRC}/shared/wx_util/wx_util.hpp 
    $ENV{CAFE_SRC}/shared/wx_util/xml_config.hpp
	$ENV{CAFE_SRC}/shared/wx_util/credit_utils.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/application_register.hpp 
    $ENV{CAFE_SRC}/shared/ksi_util/users.hpp 
    $ENV{CAFE_SRC}/shared/ksi_util/util.hpp 
    $ENV{CAFE_SRC}/shared/ksi_util/log.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/reg.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/cash_advance.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/inet.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/date_time.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/format.hpp
    $ENV{CAFE_SRC}/shared/ksi_util/currency.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/loader.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/bag_spooler.hpp
	$ENV{CAFE_SRC}/shared/ksi_util/url.hpp
	$ENV{CAFE_SRC}/shared/drawer/drawer.hpp
	$ENV{CAFE_SRC}/shared/lang/xml_lang.h
    $ENV{CAFE_SRC}/shared/shared_modules/connect.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/algo_functor.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/custom_treelistctrl.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/ksi_exceptions.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/monitor.hpp
	$ENV{CAFE_SRC}/shared/ksi_include/wx_tree_node.hpp)
SET (OTHER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/cashier.rc
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
	$ENV{CAFE_SRC}/shared/cmake_modules/miracle.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/order.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/treelistctrl.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/libpos.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/vfd_display.cmake
	$ENV{CAFE_SRC}/shared/cmake_modules/fiscal.cmake
	$ENV{CAFE_SRC}/shared/shared_modules/cashier_ico.wdr
	$ENV{CAFE_SRC}/shared/ico/cashier.ico)

IF (SOURCE_FILES)	
	foreach(file ${C_FILES} ${H_FILES} ${OTHER_FILES})
		MESSAGE (STATUS "!@# ${file}")
	endforeach(file)	
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/miracle.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/order.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/treelistctrl.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/fiscal.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libpos.cmake)
	INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/vfd_display.cmake)
	RETURN()
ENDIF (SOURCE_FILES)


PROJECT (CashierApp)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET (CMAKE_VERBOSE_MAKE_FILE ON)

INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/miracle.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libdb.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/order.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/treelistctrl.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/serial.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/fiscal.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/tinyxml.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/libpos.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/printer.cmake)
INCLUDE ($ENV{CAFE_SRC}/shared/cmake_modules/vfd_display.cmake)



FIND_LIBRARY(ORACLE_LIB_CLNTSH NAMES clntsh oci.lib PATHS $ENV{ORACLE_HOME} $ENV{ORACLE_HOME}/lib $ENV{ORACLE_HOME}/oci/lib/msvc/)
IF (NOT ORACLE_LIB_CLNTSH)
	MESSAGE(FATAL_ERROR "oracle client library not found")
ENDIF (NOT ORACLE_LIB_CLNTSH)
MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB_CLNTSH}")

IF(UNIX)
	FIND_LIBRARY(ORACLE_LIB_OCIEI ociei PATHS $ENV{ORACLE_HOME} $ENV{ORACLE_HOME}/lib)
  	IF (NOT ORACLE_LIB_OCIEI)
    		MESSAGE(FATAL_ERROR "oracle client library not found")
  	ENDIF (NOT ORACLE_LIB_OCIEI)
  	MESSAGE(STATUS "oracle lib found: ${ORACLE_LIB_OCIEI}")
ENDIF(UNIX)

IF(UNIX)
	SET(LIBPQ_LIB pq)
	ADD_DEFINITIONS(-D_UNIX_BUILD_)
ENDIF(UNIX)

IF(WIN32)
	SET(LIBPQ_LIB libpq.lib)
	ADD_DEFINITIONS(-D_WIN32_BUILD_)
ENDIF(WIN32)

FIND_LIBRARY(LIBPQ NAMES ${LIBPQ_LIB} PATHS $ENV{LIB} $ENV{POSTGRES_HOME}/lib)
IF (NOT LIBPQ)
	MESSAGE(FATAL_ERROR "PostgresQL library not found")
ENDIF (NOT LIBPQ)
MESSAGE(STATUS "Postgres lib found: ${LIBPQ}")

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

SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREAD OFF)
FIND_PACKAGE (Boost COMPONENTS system regex date_time thread)

IF (NOT Boost_FOUND)
	MESSAGE (FATAL_ERROR "boost library not found")
ENDIF (NOT Boost_FOUND)

LINK_LIBRARIES(${Boost_REGEX_LIBRARY})

IF (MINGW)
	INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIR} ${Boost_INCLUDE_DIR} $ENV{INCLUDE} $ENV{CAFE_SRC}/shared)
	ADD_DEFINITIONS (${WXWIDGETS_DEFINITIONS})

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${WXWIDGETS_CXX_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${WXWIDGETS_EXE_LINKER_FLAGS}")
ENDIF (MINGW)

FIND_LIBRARY (ORACLE_LIB NAMES oracle-s PATHS $ENV{CAFE_SRC}/shared/ksi_libdb)
IF (NOT ORACLE_LIB)
	MESSAGE (FATAL_ERROR "oracle lib not found")
ENDIF (NOT ORACLE_LIB)

FIND_LIBRARY (ORDER_LIB NAMES order-s PATHS $ENV{CAFE_SRC}/shared/ksi_order)
IF (NOT ORDER_LIB)
	MESSAGE (FATAL_ERROR "order lib not found")
ENDIF (NOT ORDER_LIB)	

FIND_LIBRARY (MIRACLE_LIB NAMES miracle-s PATHS $ENV{CAFE_SRC}/shared/miracle)
IF (NOT MIRACLE_LIB)
	MESSAGE (FATAL_ERROR "miracle lib not found")
ENDIF (NOT MIRACLE_LIB)

FIND_LIBRARY (TREELISTCTRL_LIB NAMES treelistctrl-s PATHS $ENV{CAFE_SRC}/shared/wx_util/treelistctrl)
IF (NOT TREELISTCTRL_LIB)
	MESSAGE (FATAL_ERROR "treelistctrl lib not found")
ENDIF (NOT TREELISTCTRL_LIB)

FIND_LIBRARY (POS_LIB NAMES pos-s PATHS $ENV{CAFE_SRC}/shared/ksi_libpos)
IF (NOT POS_LIB)
	MESSAGE (FATAL_ERROR "pos lib not found")
ENDIF (NOT POS_LIB)

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

FIND_LIBRARY (VFD_LIB NAMES vfd-s PATHS $ENV{CAFE_SRC}/shared/vfd_display)

IF (NOT VFD_LIB)
	MESSAGE (FATAL_ERROR "vfd lib not found")
ENDIF (NOT VFD_LIB)

FIND_LIBRARY(FISCAL_LIB NAMES fiscal-s PATHS $ENV{CAFE_SRC}/shared/fiscal_register)
IF (NOT FISCAL_LIB)
	MESSAGE (FATAL_ERROR "fiscal lib not found")
ENDIF (NOT FISCAL_LIB)

IF (WIN32)
	FIND_LIBRARY(EAY32_LIBRARY NAMES eay32 PATHS $ENV{LIB})
	IF (NOT EAY32_LIBRARY)
		MESSAGE(FATAL "eay32 library ont found")
	ENDIF (NOT EAY32_LIBRARY)
	MESSAGE(STATUS "Found eay32 library ${EAY32_LIBRARY}")
	FIND_LIBRARY(SSL32_LIBRARY NAMES ssl32 PATHS $ENV{LIB})
	IF (NOT SSL32_LIBRARY)
		MESSAGE (FATAL "ssl32 library not found")
	ENDIF (NOT SSL32_LIBRARY)
	MESSAGE(STATUS "Found ssl32 library ${SSL32_LIBRARY}")
	LINK_LIBRARIES(${SSL32_LIBRARY} ${EAY32_LIBRARY})
	IF(NOT NOT_DRAWER_BUILD)
		FIND_LIBRARY(TVICHW32_LIBRARY NAMES TVicHW32.lib PATHS $ENV{LIB})
		IF (NOT TVICHW32_LIBRARY)
			MESSAGE (FATAL "TvicHW32 library not found")
		ENDIF (NOT TVICHW32_LIBRARY)
		MESSAGE (STATUS "Found TvicHW32 library ${TVICHW32_LIBRARY}")
		LINK_LIBRARIES(${TVICHW32_LIBRARY})
	ENDIF(NOT NOT_DRAWER_BUILD)
	IF (NOT_DRAWER_BUILD)
		ADD_DEFINITIONS(-D_NOT_DRAWER_BUILD_)
	ENDIF(NOT_DRAWER_BUILD)
ENDIF (WIN32)

IF (UNIX)
	ADD_DEFINITIONS(-D_NOT_DRAWER_BUILD_)
ENDIF (UNIX)


IF (UNIX)
	FIND_LIBRARY(SSL_LIBRARY NAMES ssl PATHS $ENV{LIB})
	MESSAGE(STATUS "Found ssl library ${SSL_LIBRARY}")
ENDIF (UNIX)


IF (UNIX)
	SET (wxWidgets_USE_LIBS base core adv)
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
	ADD_EXECUTABLE (cashier ${C_FILES})
        find_library(WX_XML_LIB NAMES wx_baseu_xml-2.8 PATHS $ENV{LIB})
        MESSAGE(STATUS "Found wxXml library ${WX_XML_LIB}")
	TARGET_LINK_LIBRARIES(cashier 
        ${wxWidgets_LIBRARIES} ${ORACLE_LIB} ${ORDER_LIB} ${MIRACLE_LIB}
        ${ORACLE_LIB_CLNTSH} ${LIBPQ} ${TREELISTCTRL_LIB} ${SSL_LIBRARY}
        ${POS_LIB} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY}
        ${SERIAL_LIB} ${PRINTER_LIB} ${TINYXML_LIB} ${WX_XML_LIB} ${VFD_LIB})
ENDIF (UNIX)

IF (MINGW)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cashier_rc.o
	COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
	-i${CMAKE_CURRENT_SOURCE_DIR}/cashier.rc
	-o ${CMAKE_CURRENT_BINARY_DIR}/cashier_rc.o)

	LINK_DIRECTORIES (${Boost_LIBRARY_DIRS} ${WXWIDGETS_LINK_DIRECTORIES})
	LINK_LIBRARIES (wsock32 ws2_32 ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_REGEX_LIBRARY} ${WXWIDGETS_LIBRARIES} ${EAY32_LIBRARY} ${SSL32_LIBRARY} ${SERIAL_LIB} ${PRINTER_LIB} ${ORACLE_LIB} ${ORDER_LIB} ${MIRACLE_LIB} ${ORACLE_LIB_CLNTSH} ${LIBPQ} ${TREELISTCTRL_LIB} ${POS_LIB} ${TINYXML_LIB} ${VFD_LIB} ${FISCAL_LIB})
	
	ADD_EXECUTABLE (cashier WIN32 ${C_FILES} ${CMAKE_CURRENT_BINARY_DIR}/cashier_rc.o)
	TARGET_LINK_LIBRARIES(cashier wsock32 ws2_32 ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_REGEX_LIBRARY} ${WXWIDGETS_LIBRARIES} ${EAY32_LIBRARY} ${SSL32_LIBRARY} ${SERIAL_LIB} ${PRINTER_LIB} ${ORACLE_LIB} ${ORDER_LIB} ${MIRACLE_LIB} ${ORACLE_LIB_CLNTSH} ${LIBPQ} ${TREELISTCTRL_LIB} ${POS_LIB} ${TINYXML_LIB} ${VFD_LIB} ${FISCAL_LIB})
ENDIF (MINGW)
