cmake_minimum_required(VERSION 3.10)
project(combat_native)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "JAVA_HOME not set")
endif()

include_directories(
    $ENV{JAVA_HOME}/include
    $ENV{JAVA_HOME}/include/linux
    include
)

add_library(combat SHARED
    src/combat_engine.cpp
    src/combat_jni.cpp
    src/instance.cpp          
    src/team.cpp              
    src/player.cpp            
    src/anchors.cpp 
    src/genome_decode.cpp
)

set_target_properties(combat PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(test_build src/test_build.cpp)
target_link_libraries(test_build combat)

