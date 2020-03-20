fn main() -> int {
    a : mut int = 3;
    b := 2;

    c := a == b;
    println("c=", c);
    // println("eq: ", a == b);
    // println("gt: ", a > b);
    // println("gteq: ", a >= b);
    // println("lt: ", a < b);
    // println("lteq: ", a <= b);
    // println("nteq: ", a != b);


    if (a > 2) {
        a = 123;
    } else {
        a = 321;
    }

    d := a * 5;
    println(d);

    12.1 + 6.23;
    12.1 - 6.23;
    12.1 * 6.23;
    asd := 12.1 / 6.23;

    // println("asd=", asd);
}