fn shallow_rec(n: int) -> int {
  if (n == 0) {
    return 0;
  }
  shallow_rec(n - 1);
  println(n);
  return 0;
}


fn main() -> int {
  shallow_rec(3);
  return 0;
}