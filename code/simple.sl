fn main() -> int {
    a : mut = 30;
    b := 29;

    // println("a=", a);
    // println("b=", b);

    while (a > b) {
        a = a - 1;
        println("a=", a);
    }

    // println("final a=", a);

    return a;
}