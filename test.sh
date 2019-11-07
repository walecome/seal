#!/bin/bash
SOURCES=(
    code/main.comp
    code/expression.comp
    code/function.comp
)

ERROR_SOURCES=(
    code/illformed_types.comp
    code/error.comp
)

GREEN='\033[0;32m'
NC='\033[0m' # No Color

for i in "${SOURCES[@]}"; do
    printf "${GREEN}Running compiler for file: $i${NC}\n"
    build/compiler --source $i
    echo ""
done

for i in "${ERROR_SOURCES[@]}"; do
    printf "${GREEN}Running compiler for file (should fail): $i${NC}\n"
    build/compiler --source $i
    echo ""

done