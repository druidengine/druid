project(druid-core-test)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Duration.test.cpp
    Engine.test.cpp
    Object.test.cpp
    EnumMask.test.cpp
    Signal.test.cpp
)

enable_testing()
    
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-core
    GTest::gtest
    GTest::gtest_main
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
