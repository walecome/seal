fn main() -> int {
  a := 1;
  b := 2;
  return a + b + func_a();
}

fn func_a() -> int {
  x := 3;
  y := 4;
  z := 5;
  
  return x + y + z + func_b();
}

fn func_b() -> int {
    w := 6;
    v := 7;
    u := 8;
    return w + v + u;
}
