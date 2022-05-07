fn puts(text: string) -> void in "libc.so.6"
fn basename(env: string) -> string in "libc.so.6"
fn atoi(str: string) -> int in "libc.so.6"

fn main() -> int {
  puts("This is from puts");

  x := basename("/this/is/a/path");
  if (x != "path") {
    return 1;
  }
  println(x);

  forty := atoi("40");

  if (forty + 2 != 42) {
      return 1;
  }
  println("The answer is 42!");

  return 0;
}
