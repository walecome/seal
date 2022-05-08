fn one() -> int {
    return 1;
}

fn two() -> int {
    return 1 + 1;
}

fn three() -> int {
    return one() + two();
}

fn main() -> int {
    if (three() == 3) {
        return 0;
    }
    return 1;
}
