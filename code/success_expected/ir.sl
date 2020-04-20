fn hejsan() -> int {
    init : mut int = 100;

    var : mut int = 2;

    while (init > 10) {

        var = var + 10;
        init = init - 1;
    }

    init = 1337;

    return var;
}

fn strings() -> void {
    first : string = "hejsan";
    second : string = " world";
    third : string = first + second;
}

fn reals() -> float {
    first : float = 1.234;
    second : float = 1.337;
    return first + second;
}

fn main() -> int {

    a : mut int = 3;
    b : int = 4;

    if (a < b) {
        a = b;
    } else {
        a = b - 123;
    }

    return 0;
}

fn assigns() -> void {
    a : mut int = 10;
    a = 5;
    a = a;
}

fn func_calls() -> int{
    b : int = asd(23);

    return b + lol() * hejsan();
}


fn asd(x : int) -> int {
    a : int = 1337 - x;

    return a + 3;
}

fn lol() -> int {
    return 10;
}

fn add_with_same() -> int {

    x : int = 123;
    y : int = x + x;

    return y;

}
