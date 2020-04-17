
# Below needed for lld client and link launcher links
if(CMAKE_CXX_COMPILER_LAUNCHER STREQUAL "lld_client")

    find_program(LLD_CLIENT lld_client)

    if (NOT LLD_CLIENT)
        message(FATAL_ERROR "Could not find lld_client \$PATH.")
    endif()

    message("-- Using compiler launcher lld_client")

    add_custom_target(pre_build_target ALL
    COMMAND ${CMAKE_CXX_COMPILER} -pie -fuse-ld=/home/alexanderw/courses/exjobb/llvm-fork/build/bin/ld.lld -o sealc -Wl,--start-group -Wl,--end-group -Wl,--time-trace -Wl,--time-trace-file=timetrace.json -Wl,--project-name -Wl,${PROJECT_NAME}
    VERBATIM)

    add_dependencies(${PROJECT_NAME} pre_build_target)
    set(CMAKE_CXX_LINK_EXECUTABLE "lld_client --project-name ${PROJECT_NAME} --do-link")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --project-name ${PROJECT_NAME}")
endif()
