# write addons in Node.js

Node.js is great for writing your back-end in JavaScript.

But what if you need some functionality that is not provided out of the box, or which also can't be accomplished even using modules,
but is available in the form of a C/C++ library?

Let's see how.

it's like write native code in Java using JNI or in Android using NDK.

# let's start addons in node.js

`npm install -g node-gyp`

As for the dependencies, you will need:

- Python (2.7, 3.x will not work)
- make
- a C++ compiler toolchain (like gpp or g++)

to install all those dependencies in linux use : 

`sudo apt-get install build-essentials`

on windows you will need : 

- Python (2.7.3, 3.x will not work)
- Microsoft Visual Studio C++ 2010 (for Windows XP/Vista)
- Microsoft Visual Studio C++ 2012 for Windows Desktop (Windows 7/8)
