cmake_minimum_required (VERSION 2.8.1)

# hier setzt ihr eure Pfade zum Ordner, in den ihr LeMonADE installiert habt
SET (LEMONADE_INCLUDE_DIR "LeMonADE/lemonade_install/include/")
#cmake_policy(SET CMP0015 NEW) # cmake policy to use relative linking paths
SET (LEMONADE_LIBRARY_DIR "LeMonADE/lemonade_install/lib/")


include_directories (${LEMONADE_INCLUDE_DIR})
link_directories (${LEMONADE_LIBRARY_DIR})

# das ist das paket für den Kommandozeilenparser
find_package ( Boost REQUIRED COMPONENTS program_options)
include_directories ( ${Boost_INCLUDE_DIR} )

# das sind eure make optionen
SET (CMAKE_CXX_FLAGS "-O2 -std=c++11")

## ###############  eure Programme ############# ##
add_executable(selfAvoidingWalker main_saw.cpp)
target_link_libraries(selfAvoidingWalker LeMonADE ${Boost_LIBRARIES})

add_executable(analyzeScattering main_analyze_scattering.cpp)
target_link_libraries(analyzeScattering LeMonADE ${Boost_LIBRARIES})
