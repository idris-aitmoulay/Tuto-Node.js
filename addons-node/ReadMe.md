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

### Hello World Exemple: 
- create new Project hello Word with `npm init`
- run `npm install --save nan node-gyp`
- add all those line to `package.json` : 

```
"scripts": {
 "build": "node-gyp build",
  "rebuild": "node-gyp rebuild",
  "start": "node ./index.js"
}
```
Also note that the entry file of our package is `bindings.js`.

In this file we will expose the path to our `.node` module, 
which makes requiring it easier:

```
var myModule

if (process.env.DEBUG) {
  myModule= require('./build/Debug/myModule.node')
} else {
  myModule= require('./build/Release/myModule.node')
}

module.exports = myModule
```

In order for `node-gyp` to build our module, 
we have to give it a `binding.gyp` file where we specify the bundling instructions:

```$xslt
{
  "targets": [{
    "target_name": "myModule",
    "include_dirs" : [
      "src",
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "src/index.cc",
      "src/Vector.cc"
    ]
  }]
}
```


We will put all our header and source files in a src folder and also add the Nan headers installed in `node_modules` to the include path. Under sources you have to specify each `.cc` file that you want to compile into your module. In our simple example we will create a Vector class and expose it in the `index.cc`. 

Usually in the `index.cc` I include and initialize all the classes and cc modules, which all implement an Init method, exposing their interface to the module:


