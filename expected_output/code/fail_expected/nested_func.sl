Line 4 column 6: Nested function "nested_func" not allowed
	fn nested_func() -> int {
         return val + 10;
     }
Line 8 column 13: Call to undeclared function
	nested_func()
Line 8 column 6: Mismatched types, got int and void
	return nested_func();
