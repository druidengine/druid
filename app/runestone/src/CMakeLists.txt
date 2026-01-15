project(runestone-modules)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ChessBoard.ixx
        ChessPiece.ixx
        Move.ixx
)