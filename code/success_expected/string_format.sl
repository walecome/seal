fn main() -> int {

    formatted := fmt("{} + {} = {}", 1, 2, 3);

    println(formatted);

    other_formatted := fmt("hello world!");
    println(other_formatted);
    println(fmt("Concrete string"));

    println(fmt("{} {}", "hello", "world"));

    println(fmt("{} is [1, 2, 3]", [1, 2, 3]));
    return 0;
}
