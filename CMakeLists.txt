include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(compiler CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(PROJECT_SOURCES
#     src/ast/ArgumentList.cc
#     src/ast/ArrayLiteral.cc
#     src/ast/AssignExpression.cc
#     src/ast/BinaryExpression.cc
#     src/ast/Block.cc
#     src/ast/BooleanLiteral.cc
#     src/ast/CompilationUnit.cc
#     src/ast/Expression.cc
#     src/ast/RealLiteral.cc
#     src/ast/FunctionCall.cc
#     src/ast/FunctionDecl.cc
#     src/ast/IfStatement.cc
#     src/ast/IndexExpression.cc
#     src/ast/IntegerLiteral.cc
#     src/ast/Operator.cc
#     src/ast/ParameterList.cc
#     src/ast/ReturnStatement.cc
#     src/ast/StringLiteral.cc
#     src/ast/Type.cc
#     src/ast/UnaryExpression.cc
#     src/ast/VariableDecl.cc
#     src/ast/VariableExpression.cc
#     src/ast/While.cc
#     src/ast/For.cc

#     src/builtin/BuiltIn.cc
#     src/builtin/Print.cc

#     src/ir/Generate.cc
#     src/ir/Operand.cc
#     src/ir/IrFunction.cc
#     src/ir/IrProgram.cc
#     src/ir/Quad.cc

#     src/Error.cc
#     src/interpreter/Interpreter.cc
#     # src/TreeWalker.cc
#     src/Lexer.cc
#     src/main.cc

#     src/parse/ArgumentList.cc
#     src/parse/ArrayLiteral.cc
#     src/parse/Block.cc
#     src/parse/Expression.cc
#     src/parse/For.cc
#     src/parse/Function.cc
#     src/parse/IfStatement.cc
#     src/parse/Parser.cc
#     src/parse/ReturnStatement.cc
#     src/parse/Statement.cc
#     src/parse/Type.cc
#     src/parse/VariableDecl.cc
#     src/parse/While.cc
#     src/semantic/Scope.cc
#     src/Token.cc
#     src/TokenBuffer.cc
#     src/Util.cc
# )

# set(PROJECT_HEADERS
#     src/ast/ArgumentList.hh        
#     src/ast/ArrayLiteral.hh        
#     src/ast/AssignExpression.hh    
#     src/ast/BinaryExpression.hh    
#     src/ast/Block.hh               
#     src/ast/BooleanLiteral.hh      
#     src/ast/CompareExpression.hh   
#     src/ast/CompilationUnit.hh     
#     src/ast/Decl.hh                
#     src/ast/EqualityExpression.hh  
#     src/ast/Expression.hh
#     src/ast/RealLiteral.hh
#     src/ast/FunctionCall.hh
#     src/ast/FunctionDecl.hh
#     src/ast/IfStatement.hh
#     src/ast/IndexExpression.hh
#     src/ast/IntegerLiteral.hh
#     src/ast/Literal.hh
#     src/ast/Node.hh
#     src/ast/Operator.hh
#     src/ast/ParameterList.hh
#     src/ast/ReturnStatement.hh
#     src/ast/Statement.hh
#     src/ast/StringLiteral.hh
#     src/ast/Type.hh
#     src/ast/UnaryExpression.hh
#     src/ast/VariableDecl.hh
#     src/ast/VariableExpression.hh
#     src/ast/While.hh
#     src/ast/For.hh
    
#     src/builtin/BuiltIn.hh

    #     src/ir/Generate.hh
#     src/ir/IrFunction.hh
#     src/ir/IrProgram.hh
#     src/ir/OPCode.hh
#     src/ir/Operand.hh
#     src/ir/OperandType.hh
#     src/ir/Quad.hh

#     src/Constants.hh
#     src/Error.hh
#     src/interpreter/Interpreter.hh
#     # src/TreeWalker.hh
#     src/Lexer.hh
#     src/parse/Parser.hh
#     src/semantic/Scope.hh
#     src/SourceRef.hh
#     src/Token.hh
#     src/TokenBuffer.hh
#     src/Util.hh
# )

add_executable(${PROJECT_NAME})

include(CMakeParseArguments)

macro(add_source_files)
    cmake_parse_arguments(
        ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "PREFIX" # list of names of mono-valued arguments
        "" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
    # message("Prefix=${ARGS_PREFIX}")
    # message("Unparsed: ${ARGS_UNPARSED_ARGUMENTS}")
    # message("Prefix=${ARGS_PREFIX}")

    foreach(src ${ARGS_UNPARSED_ARGUMENTS})
        target_sources(${PROJECT_NAME} PUBLIC "${ARGS_PREFIX}/${src}")
    endforeach(src)
endmacro(add_source_files)


include_directories(lib/argparse)

add_subdirectory(src)

message("SOURCES: ${SOURCES}")

# include_directories(src/ast)
# include_directories(src/parse)
# include_directories(src/semantic)
# include_directories(src/interpreter)

# set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Weffc++ -Wnon-virtual-dtor -pedantic -fdiagnostics-color=always")

# ## Force colored output (mostly needed for Ninja build)
# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
# elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
# endif ()


# # # FMT
# add_subdirectory(lib/fmt EXCLUDE_FROM_ALL)
# target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
