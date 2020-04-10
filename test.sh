#!/bin/bash
SOURCES=(
    code/allowed_unary.sl
    code/main.sl
    code/expression.sl
    code/function.sl
    code/recursion.sl
    code/simple.sl
    code/weird_unary.sl
)

ERROR_SOURCES=(
    code/illformed_types.sl
    code/error.sl
    code/parse_missing_return_type.sl
    code/parse_missing_fn_arrow.sl
    code/parse_missing_fn_keyword.sl
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
        printf "${LIGHT_GREY}$ build/sealc --source $i${NC}\n"

        ret="$(build/sealc --source $i) " 
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