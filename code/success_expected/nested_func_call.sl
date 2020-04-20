fn main() -> int {
    println(plus_one(plus_one(plus_one(plus_one(plus_one(1))))));
    return 0;
}

fn plus_one(n: int) -> int {
    return n + 1;
}