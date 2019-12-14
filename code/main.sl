fn main() -> int {

    x : mut float = 0.1337 * 100.0;

    y : mut [int] = [0];

    for (i : mut int = 0; i < 10; ++i) {
        println("Hello world!");
    }

    x = 3.0;

    x = x * 10.0;

    y[0] = 3;

    z : mut int = y[1 * 123];

    z = y[0];

    z = 1 -(-- 3);

    println(x);

    return 0;
}

fn stupid() -> int {

    x : int = 3;
    x = 3;

}

fn top_level_func() -> int {

    fn nested_func(val : int) -> int {

        return val + 10;
    }

    return nested_func(10);
}

fn assert(value : bool, desc : string) -> void {
    if (value == false) {
        println("Assertion failed for: " + desc);
    }
}
