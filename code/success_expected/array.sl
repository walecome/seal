fn main() -> int {

    a : mut [int] = [1,2,3];

    b : int = a[0];

//    println(b);

    a[1] = 3;

    asd : mut int = 0;

    for (i : mut int = 0; i < 3; ++i) {
        tmp := a[i];
        println(tmp);
    }

    return 0;
}