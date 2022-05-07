fn fib(n : int) -> int {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }

  return fib(n - 1) + fib(n - 2);
}

fn main() -> int {
  println("Hello world!");

  limit := 20;
  n : mut = 0; // 'mut' specifier needed for mutability

  while (n <= limit) {
    result := fib(n);
    println(result);
    n = n + 1;
  }

  return 0;
}
