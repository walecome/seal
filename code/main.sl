fn main() -> int {

    print("hej");

    x : mut float = 0.1337 * 100.0;

    x = 3.0;

    x = x * 10.0;
    
    asd : int = 4;

    z : int = 1 -(--asd);

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
}

fn unary_test() -> int {
    x: mut int = 0;

    ++x;
    --x;
    x = -x;
}

fn top_level_func() -> int {

    fn nested_func(val : int) -> int {

        return val + 10;
    }

    return nested_func(10);
}

fn assert(value : bool, desc : string) -> void {
    if (value == false) {
        // println("Assertion failed for: " + desc);
    }
}
