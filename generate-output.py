#!/usr/bin/env python3

from glob import glob
import os
from subprocess import check_call, run
import re

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

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
    byte_output = run(f"{SCRIPT_DIR}/build/sealc --source {path}", shell=True, capture_output=True).stdout
    return strip_colors(byte_output.decode('utf-8'))

def build_output_path(path, output_dir):
    return output_dir + '/' + os.path.relpath(path, SCRIPT_DIR)

def main():

    target_files = list(glob(f"{SCRIPT_DIR}/code/**/*.sl"))
    print(glob(SCRIPT_DIR + "/code/**/*.sl"))
    check_call(f"{SCRIPT_DIR}/assemble", shell=True)

    output_dir = f"{SCRIPT_DIR}/expected_output"
    os.makedirs(output_dir, exist_ok=True)

    for file in target_files:
        output_file = build_output_path(file, output_dir)
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        with open(output_file, 'w') as f:
            f.write(run_file(file))

if __name__ == "__main__":
    main()
