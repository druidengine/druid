project(runestone-uci)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
        BitBoard.h
        EnumMask.h
        main.cpp
        Position.cpp
        Position.h
        Types.h
        Utils.h
)