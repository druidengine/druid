project(pong)

qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE)

set(QML_SINGLETONS
)

set_source_files_properties(${QML_SINGLETONS}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}
    URI app.${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        ${QML_SINGLETONS}
    IMPORTS
        QtQuick
    # IMPORT_PATH
    #     ${CMAKE_BINARY_DIR}/qml
)


target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Quick
)
