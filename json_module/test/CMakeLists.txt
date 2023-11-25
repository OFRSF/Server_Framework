add_executable(json_test
        json_test.cpp
)

target_link_libraries(
        json_lib
)

include(GoogleTest)
gtest_discover_tests(json_test)