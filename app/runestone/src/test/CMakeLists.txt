project(runestone-uci-test)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
        BitBoard.test.cpp
        EnumMask.test.cpp
        Position.test.cpp
        Utils.test.cpp
)

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        GTest::gtest
        GTest::gtest_main
        magic_enum::magic_enum
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
