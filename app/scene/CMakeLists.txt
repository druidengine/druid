project(druid-scene)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

# Add optimized version
project_add_executable(${PROJECT_NAME}-optimized)

target_sources(${PROJECT_NAME}-optimized PRIVATE
    main_optimized_clean.cpp
    batched_renderer.hpp
)

# Add dynamic update example
project_add_executable(${PROJECT_NAME}-dynamic)

target_sources(${PROJECT_NAME}-dynamic PRIVATE
    dynamic_update_example.cpp
    batched_renderer.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-core
    druid-graphics
)

target_link_libraries(${PROJECT_NAME}-optimized PRIVATE
    druid-core
    druid-graphics
)

target_link_libraries(${PROJECT_NAME}-dynamic PRIVATE
    druid-core
    druid-graphics
)
