[BPlusTree](https://github.com/zcbenz/BPlusTree/tree/82e2dfc3c54a2488e3a96fc356ca1af7c569098f)
==========

This is my simple implementation of B+ Tree, the keys, values, and nodes are of
fixed size, and you need to recompile when changing some important parameters.

The main advantages of my implementation is:

 * Store data in file, can manipulate huge data
 * Code is very small, indeed smaller than even most in-memory implementations
 * Has good test code (longer than implementation itself)

Feel free to use my code, I would be happy if it helps you :-)

Files
-----

`bpt.h` and `bpt.cc` is the implementation of B+ tree. `predefined.h` 
defines the tree order, key/value type, key compare function and other
tree settings, modify it to satify your need. Just include these tree files 
in your project to use the B+ tree.

License
-------

The MIT License (MIT)

Copyright (c) 2012 Zhao Cheng

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
