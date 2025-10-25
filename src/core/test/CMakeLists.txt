project(druid-core-test)

project_add_test(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Engine.test.cpp
    Entity.test.cpp
    Object.test.cpp
    EnumMask.test.cpp
    Signal.test.cpp
)
    
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-core
    GTest::gtest
    GTest::gtest_main
)
