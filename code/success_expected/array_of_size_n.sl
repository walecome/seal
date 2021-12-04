
fn main() -> int {
  // print(100);

  N := 100;

  arr2 : mut [int] = create_array(N);


  for (i : mut int = 0; i < N; ++i) {
    arr2[i] = i * 2;
  }

  for (i : mut int = 0; i < N; ++i) {
    println(arr2[i]);
  }

  return 0;
}
