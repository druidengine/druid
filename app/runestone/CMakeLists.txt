enable_language(CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(runestone)

#include(GoogleTest)
#include(CTest)

#set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
#include_directories(${PROJECT_INCLUDE_DIR})

add_subdirectory(src)