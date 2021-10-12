#!/usr/bin/bash

set -e

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_DIR="${SCRIPT_DIR}/build"

function run_from {
    local location=$1
    local cmd=$2
    cd $location
    eval "$cmd"
}

function clean {
    run_from "$SCRIPT_DIR" "rm -rf $BUILD_DIR"
}

function configure {
    clean
    cd $SCRIPT_DIR
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -GNinja ..
    cd $SCRIPT_DIR
}

function build {
    if [ ! -d "$SCRIPT_DIR/build/" ]; then
        configure
    fi
    local core_count=$(nproc)
    echo "Using ${core_count} compilation jobs"
    echo $(pwd)
    run_from "$BUILD_DIR" "ninja -j${core_count}"
    cd "$SCRIPT_DIR"
}

function print_help {
    echo "Usage ./b <clean|configure|build|help>"
}

function error_and_exit {
    echo "Invalid args: $all_args"
    exit 1
}

if [ -z "$1" ]; then
    build
    exit
fi

case "$1" in
    clean)
        clean
        ;;
    configure)
        configure
        ;;
    build)
        build
        ;;
    help)
        print_help
        ;;
    *)
        error_and_exit
        ;;
esac
