fn main() -> int {

    arr : [int] = create_array(12);

    len : mut int = get_length(arr);
    println(len);

    if (len != 12) {
        return 1;
    }

    arr2 : [int] = [1,2,3];

    len = get_length(arr2);
    println(len);
    if (get_length(arr2) != 3) {
        return 1;
    }

    str := "hejsan";

    len = get_length(str);
    println(len);
    if (get_length(str) != 6) {
        return 1;
    }


    return 0;
}
