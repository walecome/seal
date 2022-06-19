
fn main() -> int {
  i : mut int = 0;
  while (true) {
    base := ["hej", "san"];
    add_element(base, "!");

    hejsan := base[0] + base[1] + base[2];

    println(hejsan, i);
    ++i;
  }
}
