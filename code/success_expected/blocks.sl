fn main() -> int {
    a := 0;
    {
        a : mut = 1;
        {
            a = 2;
        }
    }

    return a;
}
