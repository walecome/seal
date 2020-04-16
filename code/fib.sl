fn main() -> int {

    value := fib(20);
    println(value);

    return 0;
}

fn recursive(n : int) -> int {
    println("Recursion depth: ", n);
    return recursive(n + 1);
}


fn fib(n : int) -> int {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    return fib(n - 1) + fib(n - 2);
}
