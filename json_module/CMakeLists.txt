cmake_minimum_required(VERSION 3.22)
project(JSON_LIB)

set(JSON_LIB_ROOT_DIR ${CMAKE_SOURCE_DIR}/json_module)

include_directories(${JSON_LIB_ROOT_DIR}/inc)

message(STATUS "root dir: ${JSON_LIB_ROOT_DIR}")

set(SOURCE_FILES "src/json_common.cpp")
set(HEADER_FILES "inc/json_common.h")

add_library(rf_json_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

add_subdirectory(test)

