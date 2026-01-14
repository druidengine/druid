project(runestone-test)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    ChessBoard.test.cpp
    ChessPiece.test.cpp
    Move.test.cpp
)

find_package(GTest CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest
    GTest::gtest_main
    magic_enum::magic_enum
    runestone-modules
)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
