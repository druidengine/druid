project(druid-core)

find_package(Microsoft.GSL CONFIG REQUIRED)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Components.ixx
        Concepts.ixx
        Duration.ixx
        Engine.ixx
        EnumMask.ixx
        Event.ixx
        File.ixx
        Object.ixx
        Signal.ixx
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Microsoft.GSL::GSL
)
