fn main() -> int {

    chars_printed := println(fib(20));

    println("Printed ", chars_printed, " chars");

    return 0;
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
