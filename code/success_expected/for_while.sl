fn main() -> int {

    a : mut int = 0;

    b : mut int = 2;

    while (a < 5) {
        b = b * 2;
        ++a;
    }

    c : mut int = 0;
    d : mut int = 2;

    for (i : mut int = 0; i < 5; ++i) {
        d = d * 2;
        c = i;
    }


    if (d == b){
        if (c == (a - 1)) {
            return 0;
        }
    }

    return 1;
}