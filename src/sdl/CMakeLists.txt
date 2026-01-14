project(druid-sdl)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET CXX_MODULES FILES
        Window.ixx
)

find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        druid-core
        druid-graphics
        SDL3::SDL3
)
