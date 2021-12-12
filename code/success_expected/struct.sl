struct MyStruct {
    fn create() -> MyStruct;

    fn first_member_function() -> void;
    fn second_member_function() -> int;

    member_a : int;
    member_b : string;
}

impl MyStruct::create() -> MyStruct {
    return {
        member_a := 123,
        member_b := "hello world"
    };
}

impl MyStruct::first_member_function() -> void {
    println("Called from MyStruct::first_member_function()");
}

impl MyStruct::second_member_function() -> string {
    return self.member_a + get_length(self.member_b);
}


fn main() -> int {

    my_struct := MyStruct::create();
    my_struct.first_member_function();
    my_struct.second_member_function();

    return 0;
}
