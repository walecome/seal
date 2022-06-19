# A list of random TODOs that would improve the project

- [ ] Stacktrace from crashes and asserts
- [ ] Forward args from 'assemble' to 'sealc'
  - Cannot run in verbose mode through 'assemble' currently
- [ ] Add 'gdb' option to 'assemble'
  - Would be nice if the values were displayed in-line
- [ ] Add tool to ensure consistent formatting
- [ ] Add clang-tidy checks
- [ ] Add commit hook for formatting
  - Would be nice to run lint checks
- [ ] Upgrade to C++20
- [ ] Add string formatting
  - To print/println at least


# Done
- [x] Run tests with ASAN
  - Added a flag for compiling with ASAN, which is on by default
- [x] Add signal handler to CrashHelper
- [x] Apply relocations to quads, specifically FunctionOperand and LabelOperand
      The interpreter should not know if there constructs, and can easily work with them
      if they were InstructionAddress instances, as it would not need to resolve things.
- [x] Print constant table when running with --verbose flag
