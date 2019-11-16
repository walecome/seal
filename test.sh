#!/bin/bash
SOURCES=(
    code/main.comp
    code/expression.comp
    code/function.comp
    code/weird_unary.comp
    code/recursion.comp
    code/simple.comp
)

ERROR_SOURCES=(
    code/illformed_types.comp
    code/error.comp
    code/parse_missing_return_type.comp
    code/parse_missing_fn_arrow.comp
    code/parse_missing_fn_keyword.comp
)

RED='\033[1;31m'
GREEN='\033[1;32m'
LIGHT_GREY='\033[0;37m'
NC='\033[0m' # No Color


expected=0

# Set CWD to script dir
cd "${0%/*}"


function run_and_print() {
   arr=("$@")
   for i in "${arr[@]}";
      do
        printf "${LIGHT_GREY}$ build/compiler --source $i${NC}\n"

        ret="$(build/compiler --source $i) " 
        status=$?
        if [ "$status" == "$expected" ]
        then
            printf "[${GREEN}PASSED${NC}]\n"
        else
            printf "[${RED}FAILED${NC}] - (Expected status $expected, got $status)\n"
            printf "$ret\n"
        fi
      done
}


echo ""
printf "################################# \n"
printf "#   RUNNING FOR PASSING FILES   #\n"
printf "################################# \n"
echo ""
expected=0
run_and_print "${SOURCES[@]}"

echo ""
printf "################################# \n"
printf "#   RUNNING FOR FAILING FILES   #\n"
printf "################################# \n"
echo ""
expected=1
run_and_print "${ERROR_SOURCES[@]}"


# printf "${GREEN} RUNNING FOR FAILING FILES"
# run_and_print $ERROR_SOURCES

# for i in "${ERROR_SOURCES[@]}"; do
#     printf "${GREEN}$ build/compiler --source $i${NC}\n"
    
#     build/compiler --source $i
#     echo ""

# done