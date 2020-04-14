cmake_minimum_required(VERSION 3.10)
project(sealc CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-g -fPIC -Wall -Wextra -Weffc++ -Wnon-virtual-dtor -pedantic -fdiagnostics-color=always -stdlib=libc++")

include(CMakeParseArguments)
macro(add_source_files)
    cmake_parse_arguments(
        ARGS
        ""
        "PREFIX"
        ""
        ${ARGN}
    )

    foreach(src ${ARGS_UNPARSED_ARGUMENTS})
        target_sources(${PROJECT_NAME} PUBLIC "${ARGS_PREFIX}/${src}")
    endforeach(src)
endmacro(add_source_files)

add_executable(${PROJECT_NAME})
include_directories(lib/argparse)
add_subdirectory(src)


# FMT
add_subdirectory(lib/fmt EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt-header-only)

target_compile_features(${PROJECT_NAME} PUBLIC)



# Force colored output (mostly needed for Ninja build)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif ()


# Below needed for lld client and link launcher links
if(CMAKE_CXX_COMPILER_LAUNCHER STREQUAL "link_launcher")
    message("Using link_launcher")
    set(CMAKE_CXX_LINK_EXECUTABLE "link_launcher --project-name ${PROJECT_NAME} --do-link")

    add_custom_target(pre_build_target ALL
                  COMMAND ${CMAKE_CXX_COMPILER} -pie -fuse-ld=/home/alexanderw/courses/exjobb/llvm-fork/build/bin/ld.lld -o sealc -Wl,--start-group -Wl,--end-group -Wl,--time-trace -Wl,--time-trace-file=timetrace.json -Wl,--project-name -Wl,${PROJECT_NAME}
                  VERBATIM)

    add_dependencies(${PROJECT_NAME} pre_build_target)


    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --project-name ${PROJECT_NAME}")
endif()
