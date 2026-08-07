find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(basic_tests main_test.cpp)
target_link_libraries(basic_tests PRIVATE app_core GTest::gtest GTest::gtest_main)
gtest_discover_tests(basic_tests DISCOVERY_TIMEOUT 30)
