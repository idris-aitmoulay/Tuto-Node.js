# write addons in Node.js

Node.js is great for writing your back-end in JavaScript.

But what if you need some functionality that is not provided out of the box, or which also can't be accomplished even using modules,
but is available in the form of a C/C++ library?

Let's see how.

it's like write native code in Java using JNI or in Android using NDK.

### Node Addons Module (definition)

Node.js Addons are dynamically-linked shared objects, written in c or c++, that can be loaded into Node.js using the require() function, and used just as if ther were an ordinary Node.js module. - Node.js Docs -

### Advantages (Why to use Addons ?)

 - Performance.
 - Lower Level APIs.
 - access to C++ Libraries.

## Creating an Addons

### Pre-requisites
- V8
- Libuv
- internal Node.js libraries

### Setup
- write `.cc` file
- use node-gyp  `npm install -g node-gyp`
- use `binding.gyp` file
- run `node-gyp configure build`
- at final require addon : 

`const addon = require(‘./build/Release/addon’);`

