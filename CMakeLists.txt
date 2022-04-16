cmake_minimum_required(VERSION 3.10)
project(sealc CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-g -O2 -fPIC -Wall -Wextra -Weffc++ -Wnon-virtual-dtor -pedantic -fdiagnostics-color=always")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl")

find_program(MOLD_LINKER mold)

if(MOLD_LINKER)
  message("Using mold linker: ${MOLD_LINKER}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
endif()


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

find_library(libffi_location NAMES libffi.so)
message(STATUS ${libffi_location})

add_library(ffi SHARED IMPORTED)
set_target_properties(ffi PROPERTIES IMPORTED_LOCATION ${libffi_location})

target_link_libraries(${PROJECT_NAME} PUBLIC ffi)

# Force colored output (mostly needed for Ninja build)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif()


include(LldClient.cmake)
