fn main() -> int {
    a := 0;
    println(a);
    {
        a : mut = 1;
        println(a);
        {
            a = 2;
            println(a);
        }
        println(a);
    }
    println(a);

    return a;
}
