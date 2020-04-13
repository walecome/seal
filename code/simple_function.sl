fn main() -> int {
    prints_hello();
    prints_argument(123);

    return 1;
}

fn prints_hello() -> void {
    println("Hello");
}

fn prints_argument(arg : int) -> void {
    println("Argument is: ", arg);
}