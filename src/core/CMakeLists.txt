project(druid-core)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Concepts.ixx
        Engine.ixx
        EnumMask.ixx
        Entity.ixx
        Event.ixx
        File.ixx
        Object.ixx
        Signal.ixx
)

find_package(PalSigslot CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Pal::Sigslot
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
)

# target_precompile_headers(${PROJECT_NAME} PUBLIC "<flecs.h>")