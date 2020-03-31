include_directories(.)
cmake_minimum_required(VERSION 3.10)
project(compiler CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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


set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Weffc++ -Wnon-virtual-dtor -pedantic -fdiagnostics-color=always")

# Force colored output (mostly needed for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()


# FMT
add_subdirectory(lib/fmt EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
