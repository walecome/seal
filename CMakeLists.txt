include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(compiler CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Lexer.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Token.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/TokenBuffer.cc

   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Function.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/FunctionDecl.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Parameter.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/ParameterList.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Type.cc
)

set(PROJECT_HEADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Lexer.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Token.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/TokenBuffer.hh

   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Block.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Function.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/FunctionDecl.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Node.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Parameter.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/ParameterList.hh

   # @TODO: Probably move this somewhere else
   ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/Type.hh
)


source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

include_directories(lib/argparse)


set(CMAKE_CXX_FLAGS "-g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -fdiagnostics-color=always")

## Force colored output (mostly needed for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

## Move compile_commands.json to project directory after make
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_HOME_DIRECTORY}/compile_commands.json)


find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*")
endif()
