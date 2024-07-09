cmake_minimum_required(VERSION 3.20)
project(blueprint)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    ${LIB_DIR}/blueprint.cpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(LIB_SUFFIX "x86_64")
else()
    set(LIB_SUFFIX "aarch64")
endif()

add_library(blueprint SHARED ${SOURCES})

set_target_properties(blueprint PROPERTIES OUTPUT_NAME "blueprint-${LIB_SUFFIX}")

target_include_directories(blueprint PUBLIC ${INC_DIR})
