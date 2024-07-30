cmake_minimum_required(VERSION 3.20)
project(blueprint)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

set(SOURCES
    ${LIB_DIR}/Blueprint.cpp
    ${LIB_DIR}/Schema.cpp
    ${LIB_DIR}/JSON/Token.cpp
    ${LIB_DIR}/JSON/Parser.cpp
    ${LIB_DIR}/JSON/Lexer.cpp
    ${LIB_DIR}/JSON/primitives/Number.cpp
    ${LIB_DIR}/JSON/primitives/Boolean.cpp
    ${LIB_DIR}/JSON/primitives/String.cpp
    ${LIB_DIR}/JSON/primitives/Null.cpp
    ${LIB_DIR}/JSON/primitives/Array.cpp
    ${LIB_DIR}/JSON/primitives/Object.cpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(LIB_SUFFIX "x86_64")
else()
    set(LIB_SUFFIX "aarch64")
endif()

add_subdirectory(${EXT_DIR}/fmt)

add_library(blueprint SHARED ${SOURCES})

set_target_properties(blueprint PROPERTIES OUTPUT_NAME "blueprint-${LIB_SUFFIX}")

target_include_directories(blueprint PUBLIC ${INC_DIR})
target_include_directories(blueprint PRIVATE ${EXT_DIR}/fmt/include)