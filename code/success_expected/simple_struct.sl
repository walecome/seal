struct MyStruct {
    member_a : int;
    member_b : string;
}

struct MyOtherStruct {
    first_member : int;
}

fn create_struct_tmp() -> MyStruct {
    tmp : MyStruct = { member_a := 123, member_b := "hello world" };
    // Ideally we would want to be able to omit the explicit type declaration
    // as it should be resolved by return type:
    // tmp := { member_a := 123, member_b := "hello world" };
    return tmp;
}

fn create_struct() -> MyStruct {
    return { member_a := 123, member_b := "hello world" };
}

fn create_another_struct() -> MyOtherStruct {
    return { first_member := 321 };
}

fn main() -> int {

    my_struct := create_struct;
    println(my_struct.member_a);
    println(my_struct.member_b);

    return 0;
}
