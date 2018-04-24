cmake_minimum_required (VERSION 2.6)

project (uebung2)

SET(CMAKE_CXX_FLAGS "-O2 -std=c++11 -Wall -pedantic" )
SET(CMAKE_C_FLAGS "-O2 -std=c++11") # -Wall -pedantic" )

add_library(ising_simulator ising_simulator.cpp)

#add Ising executable and link with both libraries
add_executable(Ising main_ising.cpp)
target_link_libraries(Ising ising_simulator)