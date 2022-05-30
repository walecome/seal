
# Notes for not leaking value

Each call frame gets its own fixed size register window. We know at quad generation exactly how many registers each call frame will use.

The ValuePool construct should be redesigned into just a constant pool, together with a builder object to build that constant pool.

Runtime values should not be placed in a ValuePool. I think we can have a construct called like RegisterWindow, which is a value owner for a given call frame. The actual value storage can optionally be allocated together with the rest of the RegisterWindow's to improve caching, but that feels like a premature optimization at the moment.

One issue that comes to mind is how to handle vectors. A vector with initial values will either be allocated in the constant pool, or have it's internal value allocated in the constant pool. If the vector is allocated in the constant pool, what is the result of adding an element to the vector? That added element cannot be allocated in the constant pool. Need to think how to solve this issue properly.
