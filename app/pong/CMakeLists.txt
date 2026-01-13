project(druid-pong)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-core
    druid-graphics
    druid-raylib
    druid-sdl
)
