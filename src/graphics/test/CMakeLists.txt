project(druid-graphics-test)

project_add_test(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Node.test.cpp
)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-graphics
    GTest::gtest
    GTest::gtest_main
)
