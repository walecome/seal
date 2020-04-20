fn explicit() -> void {
    x : int = 3;
    y : string = "string";
    z : float = 13.37;

    arr : [int] = [1, 2, 3];
}


fn inferred() -> void {
    x := 3;
    y := "string";
    z := 13.37;

    arr := [1, 2, 3];


    a : mut = 123;
    a = 5;
}

fn main() -> int {

    explicit();
    inferred();

    return 0;
}