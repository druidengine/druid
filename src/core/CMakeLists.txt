project(druid-core)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        CommandLineParser.ixx
        Concepts.ixx
        EnumMask.ixx
        File.ixx
        Object.ixx
        Signal.ixx
)

find_package(PalSigslot CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Pal::Sigslot
    Boost::program_options
)
