target_include_directories(${PROJECT_NAME} PRIVATE .)
target_include_directories(${PROJECT_NAME} PRIVATE ast)
target_include_directories(${PROJECT_NAME} PRIVATE base)
target_include_directories(${PROJECT_NAME} PRIVATE builtin)
target_include_directories(${PROJECT_NAME} PRIVATE interpreter)
target_include_directories(${PROJECT_NAME} PRIVATE ir)
target_include_directories(${PROJECT_NAME} PRIVATE parse)
target_include_directories(${PROJECT_NAME} PRIVATE semantic)
target_include_directories(${PROJECT_NAME} PRIVATE utility)

add_subdirectory(ast)
add_subdirectory(base)
add_subdirectory(builtin)
add_subdirectory(dynlib)
add_subdirectory(interpreter)
add_subdirectory(ir)
add_subdirectory(parse)
add_subdirectory(semantic)
add_subdirectory(utility)


add_source_files(PREFIX ${CMAKE_CURRENT_LIST_DIR}
    Constants.cc
    Error.cc
    Lexer.cc
    main.cc
    Token.cc
    TokenBuffer.cc
    Util.cc
)
