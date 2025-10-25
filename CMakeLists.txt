cmake_minimum_required(VERSION 3.31)

include(FetchContent)

# Acquire VCPKG
FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
    GIT_TAG 2025.09.17
)

FetchContent_MakeAvailable(vcpkg)

project(druid LANGUAGES CXX)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(cmake/project_compile_options.cmake)
include(cmake/project_add_library.cmake)
include(cmake/project_add_executable.cmake)
include(cmake/project_add_test.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/installed)
set(CMAKE_CXX_STANDARD 26)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include(CTest)
include(GoogleTest)

option(BUILD_ENABLE_HARDENED "Enable hardened compiler options" OFF)
option(BUILD_TESTING "Enable building ${PROJECT_NAME} test applications." ON)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_INCLUDE_DIR})

# Define build directory includes as system to avoid tools like clang-tidy from scanning them.
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/include)

set(PROJECT_TEST_EXECUTABLES "")

add_subdirectory(app)
add_subdirectory(src)

# set(PROJECT_CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_ROOT_NAME}-config-version.cmake)
# write_basic_package_version_file(${PROJECT_CONFIG_VERSION} VERSION 0.0.0 COMPATIBILITY SameMajorVersion)

# install(EXPORT ${PROJECT_ROOT_NAME}-config DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})
# install(DIRECTORY include/${PROJECT_ROOT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES ${PROJECT_CONFIG_VERSION} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})

message(STATUS "Tests registered: ${PROJECT_TEST_EXECUTABLES}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    include(ProcessorCount)
    ProcessorCount(NPROC)
    add_custom_target(clang-tidy
        COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} -j ${NPROC} -source-filter="^${CMAKE_SOURCE_DIR}/\(app|src|module\)/.*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on all files"
    )

    add_custom_target(llvm-coverage
        COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/llvm_coverage.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
