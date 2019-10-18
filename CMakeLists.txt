include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(compiler CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCES
    src/main.cc

    src/Error.cc
    src/Interpreter.cc
    src/Lexer.cc
    src/Token.cc
    src/TokenBuffer.cc
    src/Util.cc

    src/parse/ArgumentList.cc
    src/parse/Block.cc
    src/parse/Expression.cc
    src/parse/Function.cc
    src/parse/IfStatement.cc
    src/parse/Parser.cc
    src/parse/Statement.cc
    src/parse/VariableDecl.cc
    src/parse/While.cc
    src/parse/ReturnStatement.cc
    src/parse/ArrayLiteral.cc


    src/ast/ArgumentList.cc
    src/ast/AssignExpression.cc
    src/ast/Block.cc
    src/ast/FunctionDecl.cc
    src/ast/Operator.cc
    src/ast/ParameterList.cc
    src/ast/Type.cc
    src/ast/ArrayLiteral.cc
    src/ast/IntegerLiteral.cc
    src/ast/StringLiteral.cc
    src/ast/BooleanLiteral.cc
    src/ast/CompilationUnit.cc
    src/ast/FunctionCall.cc
    src/ast/VariableExpression.cc
    src/ast/BinaryExpression.cc
    src/ast/IfStatement.cc
    src/ast/VariableDecl.cc
    src/ast/While.cc
    src/ast/ReturnStatement.cc
    src/ast/UnaryExpression.cc

    src/semantic/Scope.cc
)

set(PROJECT_HEADERS

    src/ast/ArgumentList.hh        
    src/ast/ArrayLiteral.hh        
    src/ast/AssignExpression.hh    
    src/ast/BinaryExpression.hh    
    src/ast/Block.hh               
    src/ast/BooleanLiteral.hh      
    src/ast/CompareExpression.hh   
    src/ast/CompilationUnit.hh     
    src/ast/Decl.hh                
    src/ast/EqualityExpression.hh  
    src/ast/Expression.hh
    src/ast/FunctionCall.hh
    src/ast/FunctionDecl.hh
    src/ast/IfStatement.hh
    src/ast/IntegerLiteral.hh
    src/ast/Literal.hh
    src/ast/Node.hh
    src/ast/Operator.hh
    src/ast/ParameterList.hh
    src/ast/ReturnStatement.hh
    src/ast/Statement.hh
    src/ast/StringLiteral.hh
    src/ast/Type.hh
    src/ast/UnaryExpression.hh
    src/ast/VariableDecl.hh
    src/ast/VariableExpression.hh
    src/ast/While.hh
    src/Constants.hh
    src/Error.hh
    src/Interpreter.hh
    src/Lexer.hh
    src/parse/Parser.hh
    src/semantic/Scope.hh
    src/SourceRef.hh
    src/Token.hh
    src/TokenBuffer.hh
    src/Util.hh

)

include_directories(lib/argparse)

include_directories(src)
include_directories(src/ast)
include_directories(src/parse)
include_directories(src/semantic)

set(CMAKE_CXX_FLAGS "-g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -fdiagnostics-color=always")

## Force colored output (mostly needed for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})


# target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

## Move compile_commands.json to project directory after make
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_HOME_DIRECTORY}/compile_commands.json)


# find_program(
#     CLANG_TIDY_EXE
#     NAMES "clang-tidy"
#     DOC "Path to clang-tidy executable"
# )
# if(NOT CLANG_TIDY_EXE)
#     message(STATUS "clang-tidy not found.")
# else()
#     message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
#     set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*")
# endif()
# if(CLANG_TIDY_EXE)
#   set_target_properties(
#     compiler PROPERTIES
#     CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
#   )
# endif()
