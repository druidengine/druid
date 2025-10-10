cmake_minimum_required(VERSION 3.31)

project(druid LANGUAGES CXX)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(cmake/project_compile_options.cmake)
include(cmake/project_add_library.cmake)
include(cmake/project_add_executable.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/installed)
set(CMAKE_CXX_STANDARD 26)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_AUTOMOC ON)

include(CTest)
include(GoogleTest)

option(BUILD_TESTING "Enable building ${PROJECT_NAME} test applications." ON)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QT_NO_PRIVATE_MODULE_WARNING ON)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

# Set Qt policies to suppress warnings and use modern behavior
qt_policy(SET QTP0001 NEW)  # Use ':/qt/qml/' as default resource prefix for QML modules
qt_policy(SET QTP0004 NEW)  # Require qmldir files for each extra directory with .qml files

add_subdirectory(app)

# set(PROJECT_CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_ROOT_NAME}-config-version.cmake)
# write_basic_package_version_file(${PROJECT_CONFIG_VERSION} VERSION 0.0.0 COMPATIBILITY SameMajorVersion)

# install(EXPORT ${PROJECT_ROOT_NAME}-config DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})
# install(DIRECTORY include/${PROJECT_ROOT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES ${PROJECT_CONFIG_VERSION} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_custom_target(clang-tidy
        COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} -j 8 -source-filter="^${CMAKE_SOURCE_DIR}/\(app|src|module\)/.*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on all files"
    )
endif()
