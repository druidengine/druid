project(druid-skia)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET CXX_MODULES FILES
        Renderer.ixx
)

find_package(Skia CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        druid-core
        druid-graphics
        skia::skia
)
