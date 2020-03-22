fn main() -> int {
    a : mut int = 3;
    c := 1337;

    if (a > 2) {
        a = 123;
    }
    else {
        a = 321;
    }

    println(c);

    b := a + 4;
    println(b);
}