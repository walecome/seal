fn main() -> int {

    a : mut [int] = [1,2,3];

    b : int = a[0];

//    println(b);

    println("a before: ", a);

    a[1] = 3;

    asd : mut int = 0;
    
    println("a after: ", a);

    for (i : mut int = 0; i < 3; ++i) {
        tmp := a[i];
        println(tmp);
    }

    symbolic();

    return 0;
}

fn symbolic() -> void {
    a := 1;
    b := 2;
    c := 3;

    x := [a, b, c];
}
