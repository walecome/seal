fn main() -> int {

   a := "foobar";

   for (i : mut int = 0; i < 6; ++i) {
       current := a[i];
       println(current);
       if (i == 0 && current != "f") {
          return 1;
       }
       if (i == 1 && current != "o") {
          return 1;
       }
       if (i == 2 && current != "o") {
          return 1;
       }
       if (i == 3 && current != "b") {
          return 1;
       }
       if (i == 4 && current != "a") {
          return 1;
       }
       if (i == 5 && current != "r") {
          return 1;
       }
   }

   return 0;
}
