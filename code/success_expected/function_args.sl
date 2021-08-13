fn plusone(arg: int) -> int {
  return_value := arg + 1;
  println("Will return ", return_value);
  return return_value;
}

fn main() -> int {
  five := 5;
  six: int = plusone(five);
  return six - 6;
}