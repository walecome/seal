fn puts(text: string) -> void in "libc.so.6"
fn printf(fmt: string, arg: string) -> void in "libc.so.6"
fn basename(env: string) -> string in "libc.so.6"
fn atoi(str: string) -> int in "libc.so.6"

fn main() -> int {
  // printf("%s\n", "This is from printf!");
  puts("This is from puts");
  puts("This is another line from puts!");

  x := basename("/this/is/a/path");
  if (x != "path") {
    return 1;
  }
  println(x);

  asd := atoi("3");
  println(asd + 5);


  return 0;
}
