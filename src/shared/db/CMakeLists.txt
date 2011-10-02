cmake_minimum_required(VERSION 2.6)
PROJECT(ORACLE_DRIVER_LIBRARY)
include_directories($ENV{ORACLE_HOME}/sdk/include $ENV{ORACLE_HOME}/include $ENV{ORACLE_HOME}/rdbms/public)
link_directories($ENV{ORACLE_HOME}/ $ENV{ORACLE_HOME}/lib)
find_library(ORA_CLNTSH clntsh PATHS $ENV{ORACLE_HOME}/ $ENV{ORACLE_HOME}/lib)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)

find_package( Boost )
include_directories(${Boost_INCLUDE_DIRS})

add_library(oracle-s oracle_driver.cpp)

if (BUILD_TEST)
  add_executable(driver_test ./test/driver_test.cpp oracle_driver.cpp)
  target_link_libraries(driver_test ${ORA_CLNTSH})
endif (BUILD_TEST)

if (BUILD_SHARED)
  add_library(oracle SHARED oracle_driver.cpp)
  target_link_libraries(oracle ${ORA_CLNTSH})
endif(BUILD_SHARED)
