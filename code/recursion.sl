fn main() -> int {
    recursive(0);

    return 0;
}

fn recursive(n : int) -> int {
    println("Recursion depth: ", n);
    return recursive(n + 1);
}