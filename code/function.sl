fn main() -> int {
    print("fib");
    asd := fib(10);
    print(asd);
    return 0;
}


fn myfunc(var1: int) -> void {
    1 - 2;
    asd : mut int = (1);
    hejsan : int = (3 + 5) * 2;
    var2 : int = asd * (asdfunc(1) + 5);
    abc(asdfunc(1) * (asd - var2));

    x : mut int = 0;

    if (asd < var2) {
        x = asdfunc(asd);
        asd = var2;
    } else {
        x = 5;
    }

    {
        x = 3;
        x : int = 5;
    }

    my_bool : bool = true;

    if (x == 3) {print("xd");}


    functioncall(var2);
    functioncallwithint(1);
    functioncallwithstring("my string!");

    var : int = 15;

    if (asd == var) {
        print("some print!");
    }

    while (asd > var2) {
        print("something fun");
    }
}

fn functioncall(a : int) -> void {}
fn functioncallwithint(a : int) -> void {}
fn functioncallwithstring(a : string)  -> void {}
fn asdfunc(a : int) -> int {
    return a + 3;
}
fn abc(a : int) -> string {
    print("hello world");
    return "Hello world!";
}

fn rec_test(n : int) -> int {
    print(n);
    if (n == 0) {
        return 0;
    }

    return rec_test(n - 1);
}

fn print(text : string) -> void {}

fn fib(n: int) -> int {
    if (n <= 1) {
        return n;
    }

    return fib(n - 1) + fib(n - 2);
}

fn empty() -> void {}