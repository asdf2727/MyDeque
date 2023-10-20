# MyDeque

This was an experiment to see if I could create a decently fast deque data structure in C++. Its aim was to replace the standard std::deque <uint32_t> that I am (at the time of writing) using in my hugeint project, which can also be found on my github.

The results were interesting, and due to my simpler implementation algorithms like addition or bitwise operations ended up being faster, due to a lot of iteration and not a lot of resizing of the container.
Unfortunately, the multiplication was a bit slower, possibly due to my Karatsuba implementation, and because multiplication is an operation with bigger complexity, I ended up keeping the default std::deque class.

Thanks to Liviu23 for helping me compare the speeds of the two implementations!
