cmake_minimum_required(VERSION 3.15)

project(combat_native LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ============================================================
# OpenMP (OBLIGATORIO para paralelización)
# ============================================================
find_package(OpenMP REQUIRED)

# ============================================================
# Detect JAVA_HOME
# ============================================================
if(NOT DEFINED ENV{JAVA_HOME})
    message(WARNING "JAVA_HOME is not set. Trying to auto-detect...")
    find_package(Java REQUIRED)
    find_package(JNI REQUIRED)
else()
    set(JAVA_INCLUDE_DIRS
        $ENV{JAVA_HOME}/include
        $ENV{JAVA_HOME}/include/linux
        $ENV{JAVA_HOME}/include/darwin
        $ENV{JAVA_HOME}/include/win32
    )
endif()

# ============================================================
# Include directories
# ============================================================
include_directories(
    include
    ${JAVA_INCLUDE_DIRS}
)

# ============================================================
# Sources
# ============================================================
set(COMBAT_SOURCES
    src/anchors.cpp
    src/combat_engine.cpp
    src/combat_jni.cpp
    src/genome_decode.cpp
    src/instance.cpp
    src/team.cpp
    src/player.cpp
    src/rng.cpp
    src/rng_context.cpp
)

# ============================================================
# Shared library (JNI)
# ============================================================
add_library(combat SHARED ${COMBAT_SOURCES})

set_target_properties(combat PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin
)

target_link_libraries(combat
    PRIVATE OpenMP::OpenMP_CXX
)

# ============================================================
# Executables
# ============================================================
add_executable(combat_app src/main.cpp ${COMBAT_SOURCES})
add_executable(combat_test src/test_build.cpp ${COMBAT_SOURCES})

set_target_properties(combat_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin
)
set_target_properties(combat_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin
)

target_link_libraries(combat_app
    PRIVATE combat OpenMP::OpenMP_CXX
)

target_link_libraries(combat_test
    PRIVATE combat OpenMP::OpenMP_CXX
)

# ============================================================
# Platform defs
# ============================================================
if(WIN32)
    target_compile_definitions(combat PRIVATE WINDOWS)
elseif(APPLE)
    target_compile_definitions(combat PRIVATE MACOS)
else()
    target_compile_definitions(combat PRIVATE LINUX)
endif()

# ============================================================
# Build type
# ============================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the build type (Debug or Release)" FORCE)
endif()

message(STATUS "==========================================")
message(STATUS " Building combat-native into: ${CMAKE_CURRENT_SOURCE_DIR}/build")
message(STATUS " Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS " OpenMP enabled")
message(STATUS "==========================================")
