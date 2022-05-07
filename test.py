#!/usr/bin/env python3

import subprocess
import os
import sys
import glob
import re
import difflib

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

# https://stackoverflow.com/questions/14693701/how-can-i-remove-the-ansi-escape-sequences-from-a-string-in-python
ansi_escape = re.compile(r'''
    \x1B  # ESC
    (?:   # 7-bit C1 Fe (except CSI)
        [@-Z\\-_]
    |     # or [ for CSI, followed by a control sequence
        \[
        [0-?]*  # Parameter bytes
        [ -/]*  # Intermediate bytes
        [@-~]   # Final byte
    )
''', re.VERBOSE)

def strip_colors(text):
    return ansi_escape.sub('', text)

def run_file(path):
    byte_output = subprocess.run(f"{SCRIPT_DIR}/build/sealc --source {path}", shell=True, capture_output=True).stdout
    return strip_colors(byte_output.decode('utf-8'))

def read_expected(test_file):
    target = SCRIPT_DIR + "/expected_output/" + os.path.relpath(test_file, SCRIPT_DIR)
    with open(target, 'r') as f:
        return f.read()

def check_diffs():
    target_files = glob.glob(os.path.join(CODE_DIR, "**", "*.sl"))
    failed_diffs = []
    for file in target_files:
        actual_output = run_file(file)
        expected_output = read_expected(file)
        diff = difflib.context_diff(actual_output, expected_output)
        if len(list(diff)) != 0:
            failed_diffs.append(file)
    return failed_diffs

def print_status_and_exit(success):
    if success:
        print(f"{GREEN}All tests succeeded!{NC}")
        exit(0)
    else:
        print(f"{RED}Tests failed!{NC}")
        exit(1)


def main():
    files_with_failed_diff = check_diffs()
    if files_with_failed_diff:
        print("The following files failed diff check:")
        for file in files_with_failed_diff:
            print(f"\t{file}")
    diff_ok = len(files_with_failed_diff) == 0
    pass_ok = run_expect_pass()
    fail_ok = run_expect_fail()
    print_status_and_exit(pass_ok and fail_ok and diff_ok)

if __name__ == "__main__":
    main()
