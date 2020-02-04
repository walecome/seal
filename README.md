# (WIP) Seal programming language

### A work in progress compiler for the Seal programming language.

Current working:

- Lexing
- Parsing (Recursive descent)
- Semantic analysis (type checking, etc...)

Partially working:

- Tree walk interpreter (slow)
- Quad generation

To be implemented:

- LLVM IR generation
- Register or stack based interpreter
- LLVM integration for generating machine code

### Building:

```
git clone --recursive https://github.com/walecome/seal
cd seal && mkdir build && cd build
cmake ..
make
```

#### Fibonacci:

```
fn fib(n : int) -> int {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    return fib(n - 1) + fib(n - 2);
}

```
