cmake_minimum_required (VERSION 2.6)

project (uebung0)

SET(CMAKE_CXX_FLAGS "-O2 -Wall -pedantic" )
SET(CMAKE_C_FLAGS "-O2 -Wall -pedantic" )

add_library(lib_calc calculator.cpp)

add_executable(Calculator main_programming.cpp)

target_link_libraries(Calculator lib_calc)
