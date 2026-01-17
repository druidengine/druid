project(druid-widgets-test)

find_package(GTest CONFIG REQUIRED)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Widget.test.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-widgets
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
