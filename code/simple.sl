fn main() -> int {
    println("hej");
    12.1 + 6.23;
    12.1 - 6.23;
    12.1 * 6.23;
    asd := 12.1 / 6.23;

    println("asd=", asd);

    a:= 3;
    b:= a + 1;
    c:= b + 1;
    d:= c + 1;
    d + 5;


    func_with_args(function_returning_int(), b);
}

fn func_with_args(a: int, b: int) -> void {

}

fn function_returning_int() -> int {
    return 123;
}