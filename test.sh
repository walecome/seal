#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
LIGHT_GREY='\033[0;37m'
NC='\033[0m' # No Color


# Set CWD to script dir
cd "${0%/*}"

code_dir=./code

sealc=build/sealc


function run_one() {
    expected=$1
    source_file=$2

    printf "${YELLOW}[RUNS] ${LIGHT_GREY}$ $sealc --source $source_file${NC}\n"

    ret="$($sealc --source $source_file) " 
    status=$?
    printf "\e[1A"
    if [ "$status" == "$expected" ]
    then
        printf "[${GREEN}PASS${NC}] ${LIGHT_GREY}$ $sealc --source ${NC}\n"
    else
        printf "[${RED}FAIL${NC}] ${LIGHT_GREY}$ $sealc --source ${NC}\n"
        printf "\tExpected status $expected, got $status\n"

        printf "\tCommand output:\n$ret\n"
    fi
}

function run_and_print() {

    source_folder="$code_dir/$2"

    for f in $source_folder/*.sl
    do
        run_one $1 $f
    done
}

printf "${NC}${YELLOW}\tRUNNING EXPECTED PASS TESTS${NC}\n"
run_and_print 0 success_expected

printf "${NC}${YELLOW}\tRUNNING EXPECTED FAIL TESTS${NC}\n"
run_and_print 1 fail_expected

