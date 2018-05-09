cmake_minimum_required (VERSION 2.8.1)

# hier setzt ihr eure Pfade zum Ordner, in den ihr LeMonADE installiert habt
#SET (LEMONADE_INCLUDE_DIR "LeMonADE/lemonade_install/include/")
#cmake_policy(SET CMP0015 NEW) # cmake policy to use relative linking paths
#SET (LEMONADE_LIBRARY_DIR "LeMonADE/lemonade_install/lib/")

SET (LEMONADE_INCLUDE_DIR "/home/users/wengenmayr/workspace/lemonade/lemonade_lib/include/")
SET (LEMONADE_LIBRARY_DIR "/home/users/wengenmayr/workspace/lemonade/lemonade_lib/lib")
if (NOT DEFINED LEMONADE_INCLUDE_DIR)
message("LEMONADE_INCLUDE_DIR is not provided. Please set path/to/LeMonADE/headers/")
endif()
if (NOT DEFINED LEMONADE_LIBRARY_DIR)
message("LEMONADE_LIBRARY_DIR is not provided. Please set path/to/LeMonADE/lib/")
endif()
include_directories (${LEMONADE_INCLUDE_DIR})
link_directories (${LEMONADE_LIBRARY_DIR})

# das ist das paket für den Kommandozeilenparser
find_package ( Boost REQUIRED COMPONENTS program_options)
include_directories ( ${Boost_INCLUDE_DIR} )

# das sind eure make optionen
SET (CMAKE_CXX_FLAGS "-O2 -std=c++11")

## ###############  eure Programme ############# ##
add_executable(createIdealLinearChain main_create_ideal_chain.cpp)
target_link_libraries(createIdealLinearChain LeMonADE ${Boost_LIBRARIES})

add_executable(createRealLinearChain main_create_real_chain.cpp)
target_link_libraries(createRealLinearChain LeMonADE ${Boost_LIBRARIES})

add_executable(simulatorExcludedVolume main_real_Simulator.cpp)
target_link_libraries(simulatorExcludedVolume LeMonADE ${Boost_LIBRARIES})

add_executable(simulatorIdeal main_ideal_Simulator.cpp)
target_link_libraries(simulatorIdeal LeMonADE ${Boost_LIBRARIES})

add_executable(analyzeReeAndRg main_analyze_radius.cpp)
target_link_libraries(analyzeReeAndRg LeMonADE ${Boost_LIBRARIES})

add_executable(analyzeScattering main_analyze_scattering.cpp)
target_link_libraries(analyzeScattering LeMonADE ${Boost_LIBRARIES})
