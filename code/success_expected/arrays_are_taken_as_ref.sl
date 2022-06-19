fn add_five(arr: [int]) -> void {
  add_element(arr, 5);
}

fn main() -> int {
    arr : mut = [0,1,2];
    println(arr);
    add_five(arr);
    println(arr);
    return arr[3] - 5;
}
