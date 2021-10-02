#!/usr/bin/env python3

import subprocess
import os
import sys
import glob

RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
LIGHT_GREY='\033[0;37m'
NC='\033[0m' # No Color


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
CODE_DIR = os.path.join(SCRIPT_DIR, "code")
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
SEALC = os.path.join(BUILD_DIR, "sealc")

def print_no_endline(msg):
    print(msg, end="")
    sys.stdout.flush()

def reset_terminal_line():
    print(chr(27) + "[1A")

def decode_bytes(byte_str):
    return byte_str.decode(sys.stdout.encoding)

def run_command(command):
    try:
        return 0, decode_bytes(subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True))
    except subprocess.CalledProcessError as ex:
        return ex.returncode, decode_bytes(ex.output)

def run_one(expected_exit_code: int, source_file: str):
    compile_command = f"{SEALC} --source {source_file}"

    print_no_endline(f"{YELLOW}[RUNS] {LIGHT_GREY} {compile_command}")

    return_code, output = run_command(compile_command)
    reset_terminal_line()
    if return_code != expected_exit_code:
        print(f"[{RED}FAIL{NC}] {LIGHT_GREY} {compile_command} {NC}")
        print(f"\tExpected status {expected_exit_code}, got {return_code}")
        print(f"\tCommand output:")
        print(output)

    else:
        print(f"[{GREEN}PASS{NC}] {LIGHT_GREY} {compile_command} {NC}")
    return return_code == expected_exit_code


def run_expect_pass():
    print("Running tests with expected success")
    all_expected = True
    for file in glob.glob(os.path.join(CODE_DIR, "success_expected", "*.sl")):
        all_expected = run_one(0, file) and all_expected
    return all_expected

def run_expect_fail():
    print("Running tests with expected failure")
    all_expected = True
    for file in glob.glob(os.path.join(CODE_DIR, "fail_expected", "*.sl")):
        all_expected = run_one(1, file) and all_expected
    return all_expected


def main():
    pass_ok = run_expect_pass()
    fail_ok = run_expect_fail()
    if pass_ok and fail_ok:
        exit(0)
    else:
        exit(1)


if __name__ == "__main__":
    main()
