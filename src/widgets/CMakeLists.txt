project(druid-widgets)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Widget.ixx
)

find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    druid-core
    glm::glm
)
