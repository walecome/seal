# A list of random TODOs that would improve the project

- [ ] Make Generate produce:
  - A flat list of quads
  - A list of quads that need relocation
  - A map mapping function id to quad index
  - A map mapping label id to quad index
- [ ] Stacktrace from crashes and asserts
- [ ] Forward args from 'assemble' to 'sealc'
  - Cannot run in verbose mode through 'assemble' currently
- [ ] Add 'gdb' option to 'assemble'
- [ ] Print constant table when running with --verbose flag
  - Would be nice if the values were displayed in-line
- [ ] Add tool to ensure consistent formatting
- [ ] Add clang-tidy checks
- [ ] Run tests with ASAN
- [ ] Add commit hook for formatting
  - Would be nice to run lint checks
- [ ] Upgrade to C++20
- [ ] Add string formatting
  - To print/println at least
- [ ] Add signal handler to CrashHelper
  - Is currently only helpful on asserts failing
- [ ] Apply relocations to quads, specifically FunctionOperand and LabelOperand
      The interpreter should not know if there constructs, and can easily work with them
      if they were InstructionAddress instances, as it would not need to resolve things.
