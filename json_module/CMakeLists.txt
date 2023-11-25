cmake_minimum_required(VERSION 3.22)
project(JSONLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(JSON_ROOT_DIR ${CMAKE_SOURCE_DIR})

add_compile_options(-O0 -g -Werror -Wextra -Wall -pedantic)

include_directories(${JSON_ROOT_DIR}/inc)

add_library(json_lib
        src/json_common.cpp inc/json_common.h)

add_subdirectory(test)