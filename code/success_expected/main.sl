fn main() -> int {

    println("hej");

    x : mut float = 0.1337 * 100.0;

    x = 3.0;

    x = x * 10.0;
    
    asd : mut int = 4;

    z : int = -(--asd);

    println(x);

    lol := "hejsan";

    return 0;
}

fn prints() -> void {
    print("hejsan");
    println("hejsan");
}

fn stupid() -> int {

    x : mut int = 3;
    x = 3;

    return x;
}

fn unary_test() -> int {
    x: mut int = 0;

    ++x;
    --x;
    x = -x;

    return x;
}

fn top_level_func() -> int {

    return 10;
}

fn assert(value : bool, desc : string) -> void {
    if (value == false) {
        // println("Assertion failed for: " + desc);
    }
}
