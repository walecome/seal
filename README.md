# Seal Programming Language

Seal is a programming language featuring a hand-written recursive descent parser, infer-based type system, C-bindings, and register-based virtual machine. This is an ongoing project, which in practise means that the feature list is constantly changing, but the backlog looks something like:

- Compilation (JIT and AOT) using LLVM
- A more complete set of built-ins and a standard library
- A language server compatible with the LSP (Language Server Protocol)
- A debugger

### How to build

```sh
git clone --recursive https://github.com/walecome/seal
cd seal
./assemble # Outputs 'build/sealc' if successful
```

### Running

```sh
./assemble run mysealfile.sl # Run using 'assemble', performs build step on new changes
build/sealc --source mysealfile.sl # Run using 'sealc' directly, requires build step
```

### Examples

#### Fibonacci

```rust
fn fib(n : int) -> int {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }

  return fib(n - 1) + fib(n - 2);
}

fn main() -> int {
  println("Hello world!");

  limit := 20;
  n : mut = 0; // 'mut' specifier needed for mutability

  while (n <= limit) {
    result := fib(n);
    println(result);
    n = n + 1;
  }

  return 0;
}
```
