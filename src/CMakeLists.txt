target_include_directories(${PROJECT_NAME} PUBLIC .)
target_include_directories(${PROJECT_NAME} PUBLIC ast)
target_include_directories(${PROJECT_NAME} PUBLIC builtin)
target_include_directories(${PROJECT_NAME} PUBLIC interpreter)
target_include_directories(${PROJECT_NAME} PUBLIC ir)
target_include_directories(${PROJECT_NAME} PUBLIC parse)
target_include_directories(${PROJECT_NAME} PUBLIC semantic)

add_subdirectory(ast)
add_subdirectory(builtin)
add_subdirectory(interpreter)
add_subdirectory(ir)
add_subdirectory(parse)
add_subdirectory(semantic)


add_source_files(PREFIX ${CMAKE_CURRENT_LIST_DIR}
    Error.cc
    Lexer.cc
    main.cc
    Token.cc
    TokenBuffer.cc
    Util.cc
)