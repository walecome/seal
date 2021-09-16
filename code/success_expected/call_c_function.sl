fn puts(text: string) -> void in "libc.so.6"
fn printf(fmt: string, arg: string) -> void in "libc.so.6"
fn getenv(env: string) -> string in "libc.so.6"

fn main() -> int {
  // printf("%s\n", "This is from printf!");
  puts("This is from puts");
  puts("This is another line from puts!");

  x := getenv("hello");
  println(x);

  return 0;
}
