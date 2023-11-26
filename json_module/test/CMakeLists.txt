cmake_minimum_required(VERSION 3.22)

project(json_test)

set(SOURCE_FILES "json_test.cpp")

add_executable(json_test ${SOURCE_FILES})


target_link_libraries(json_test rf_json)
