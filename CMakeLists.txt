CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

project(OFRSF CXX)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
if(COMPILER_SUPPORTS_CXX20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support. Please use a different C++ compiler.")
endif()

add_compile_options(-O0 -g -Werror -Wextra -Wall -pedantic)


# config google test


add_subdirectory(json_module)