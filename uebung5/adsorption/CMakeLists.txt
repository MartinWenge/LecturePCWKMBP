cmake_minimum_required (VERSION 2.8.1)

# hier setzt ihr eure Pfade zum Ordner, in den ihr LeMonADE installiert habt
#SET (LEMONADE_INCLUDE_DIR "LeMonADE/lemonade_install/include/")
#cmake_policy(SET CMP0015 NEW) # cmake policy to use relative linking paths
#SET (LEMONADE_LIBRARY_DIR "LeMonADE/lemonade_install/lib/")

SET (LEMONADE_INCLUDE_DIR "/scratch/localuser/lemonade/lemonadeInstall/include/")
SET (LEMONADE_LIBRARY_DIR "/scratch/localuser/lemonade/lemonadeInstall/lib")
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
add_executable(createConfigAdsorption main_create_adsorbing_chain.cpp)
target_link_libraries(createConfigAdsorption LeMonADE ${Boost_LIBRARIES})

add_executable(simulatorAdsorptionReal main_adsorption_Simulator_real.cpp)
target_link_libraries(simulatorAdsorptionReal LeMonADE ${Boost_LIBRARIES})

add_executable(simulatorAdsorptionIdeal main_adsorption_Simulator_ideal.cpp)
target_link_libraries(simulatorAdsorptionIdeal LeMonADE ${Boost_LIBRARIES})

add_executable(analyzeAdsorption main_analyze_adsorption.cpp)
target_link_libraries(analyzeAdsorption LeMonADE ${Boost_LIBRARIES})

add_executable(analyzeRadius main_analyze_radius.cpp)
target_link_libraries(analyzeRadius LeMonADE ${Boost_LIBRARIES})



