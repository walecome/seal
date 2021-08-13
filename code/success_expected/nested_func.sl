fn top_level_func() -> int {
    val := 10;
    fn nested_func() -> int {
        return val + 10;
    }

    return nested_func();
}

fn main() -> int {
  return top_level_func() - 20;
}