#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
LIGHT_GREY='\033[0;37m'
NC='\033[0m' # No Color


# Set CWD to script dir
cd "${0%/*}"

code_dir=./code

sealc=build/sealc

function run_one() {
    expected=$1
    source_file=$2

    printf "${LIGHT_GREY}$ $sealc --source $i${NC}\n"

    ret="$($sealc --source $source_file) " 
    status=$?
    if [ "$status" == "$expected" ]
    then
        printf "[${GREEN}PASSED${NC}]\n"
    else
        printf "[${RED}FAILED${NC}] - (Expected status $expected, got $status)\n"
        printf "$ret\n"
    fi
}

function run_and_print() {

    source_folder="$code_dir/$2"

    for f in $source_folder/*.sl
    do
        run_one $1 $f
    done
}

echo ""
printf "################################# \n"
printf "#   RUNNING FOR PASSING FILES   #\n"
printf "################################# \n"
echo ""
run_and_print 0 success_expected


echo ""
printf "################################# \n"
printf "#   RUNNING FOR FAILING FILES   #\n"
printf "################################# \n"
echo ""
run_and_print 1 fail_expected

