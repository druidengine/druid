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
    druid-skia
)

# Suppress DLL interface warnings from Skia headers (C4251, C4275)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4251 /wd4275)
endif()
