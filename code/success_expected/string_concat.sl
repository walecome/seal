fn main() -> int {
   a := "first";
   b := "second";

   d : mut string = "";
   c := a + b;
   d = c;

   println(c);

   if (c == "firstsecond") {
       return 0;
   } else {
       return 1;
   }
}
