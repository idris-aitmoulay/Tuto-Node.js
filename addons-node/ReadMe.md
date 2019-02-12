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

```$xslt
#include <nan.h>
#include "Vector.h"

NAN_MODULE_INIT(InitModule) {
  Vector::Init(target);
}

NODE_MODULE(myModule, InitModule);
```

Now that we have everything set up to build and expose our C++ module, we can start to implement stuff!
##### Implementing our first class :
 
Probably the most important thing, when you are just getting started, is to understand how to write classes on the native side and how to use them in your JavaScript app. 

Let’s say we want to implement a simple Vector class which can do the following:

```$xslt
// this is how we will require our module
const m = require('./')

const vec1 = new m.Vector(20, 10, 0)
console.log(vec1) // Vector { x: 20, y: 10, z: 0 }

const vec2 = new m.Vector(0, 0, 0)
vec2.x = 30
vec2.z = 100
console.log(vec2) // Vector { x: 30, y: 0, z: 100 }

const vecSum = vec1.add(vec2)
console.log(vecSum) // Vector { x: 50, y: 10, z: 100 }
```
For now we want to initialize new Vectors with their x, y and z coordinates, we want be able to change their coordinates by assigning new values to them and finally we also want to add a Vector to another one.

##### Declaring the class :

To define the Vector class we will create a header file “Vector.h” with the following contents:

```$xslt
#include <nan.h>

class Vector : public Nan::ObjectWrap {
public:
  double x;
  double y;
  double z;

  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static NAN_METHOD(Add);

  static NAN_GETTER(HandleGetters);
  static NAN_SETTER(HandleSetters);

  static Nan::Persistent<v8::FunctionTemplate> constructor;
};
```
You can see that Vector extends the Nan::ObjectWrap class. This is required to pass instances of this class between your JavaScript app and the native module, as you pass them to the JS side by wrapping them and retrieve them on the native side by unwrapping them. We will discuss object wrapping later on in this tutorial.

Furthermore, our class will have three properties: x, y and z as well as getters and setters for their values, which you can declare with the NAN_GETTER and NAN_SETTER macros. We also declare the methods Init, New and Add. The New function, as the name implies, will be our constructor and the Init function is our “module.exports”, which is the first thing being called when we require our module from our app. It basically exposes our Vector class to our module and is declared with the NAN_MODULE_INIT macro. Methods accessible from JavaScript are declared as NAN_METHOD. Note, we have to declare everything statically, which can be invoked from JavaScript.


The last thing we have to define in our class header is a persistent handle to our constructor. You will mostly deal with local and persistent handles, which determine the lifetime of the handle. In contrast to a persistent handle, a local handle (you will see that one later on a lot) will be garbage collected after leaving the scope it is being declared in. But don’t worry about handles and their scopes for now, you don’t even have to declare HandleScopes in methods accessible from JavaScript since NAN_METHOD kindly does this for us already.


##### Implementing the class methods :

Now that we have declared our class, we can go ahead and implement our methods, which we will add to our Vector.cc file.


```$xslt
#include "Vector.h"

Nan::Persistent<v8::FunctionTemplate> Vector::constructor;

NAN_MODULE_INIT(Vector::Init) {
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Vector::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Vector").ToLocalChecked());

  // link our getters and setter to the object property
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("x").ToLocalChecked(), Vector::HandleGetters, Vector::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("y").ToLocalChecked(), Vector::HandleGetters, Vector::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("z").ToLocalChecked(), Vector::HandleGetters, Vector::HandleSetters);

  Nan::SetPrototypeMethod(ctor, "add", Add);

  target->Set(Nan::New("Vector").ToLocalChecked(), ctor->GetFunction());
}
```


This is the basic pattern I adopted from tutorials to initialize a new class. First we create the FunctionTemplate for our constructor, which we assign to the New method that we declared and assign this handle to our persistent constructor by calling Reset.


Furthermore, we set the class name, the accessors and prototype methods. SetClassName and Nan::SetAccessor expects the property name to be a v8::Local<v8::String>, a JavaScript String. The reason why we have to call ToLocalChecked() on every JS object that we create with Nan::New is, because Nan::New returns a MaybeLocal, which is a wrapper around v8::Local handles. JS objects can be empty, which MaybeLocals allow you to check for by calling IsEmpty() to catch exceptions early. In our case we create the handle ourselves so we can safely call ToLocalChecked() to return us the v8::Local handle. Lastly we expose our class to our module (target comes from the NAN_MODULE_INIT macro).

```$xslt
NAN_METHOD(Vector::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("Vector::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if(info.Length() != 3) {
    return Nan::ThrowError(Nan::New("Vector::New - expected arguments x, y, z").ToLocalChecked());
  }

  // expect arguments to be numbers
  if(!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber()) {
    return Nan::ThrowError(Nan::New("Vector::New - expected arguments to be numbers").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance
  Vector* vec = new Vector();
  vec->Wrap(info.Holder());

  // initialize it's values
  vec->x = info[0]->NumberValue();
  vec->y = info[1]->NumberValue();
  vec->z = info[2]->NumberValue();

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}
```

and also :


```$xslt
NAN_METHOD(Vector::Add) {
  // unwrap this Vector
  Vector * self = Nan::ObjectWrap::Unwrap<Vector>(info.This());

  if (!Nan::New(Vector::constructor)->HasInstance(info[0])) {
    return Nan::ThrowError(Nan::New("Vector::Add - expected argument to be instance of Vector").ToLocalChecked());
  }
  // unwrap the Vector passed as argument
  Vector * otherVec = Nan::ObjectWrap::Unwrap<Vector>(info[0]->ToObject());

  // specify argument counts and constructor arguments
  const int argc = 3;
  v8::Local<v8::Value> argv[argc] = {
    Nan::New(self->x + otherVec->x),
    Nan::New(self->y + otherVec->y),
    Nan::New(self->z + otherVec->z)
  };

  // get a local handle to our constructor function
  v8::Local<v8::Function> constructorFunc = Nan::New(Vector::constructor)->GetFunction();
  // create a new JS instance from arguments
  v8::Local<v8::Object> jsSumVec = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();

  info.GetReturnValue().Set(jsSumVec);
}
```
and accessor :


````$xslt
NAN_GETTER(Vector::HandleGetters) {
  Vector* self = Nan::ObjectWrap::Unwrap<Vector>(info.This());

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "x") {
    info.GetReturnValue().Set(self->x);
  } else if (propertyName == "y") {
    info.GetReturnValue().Set(self->y);
  } else if (propertyName == "z") {
    info.GetReturnValue().Set(self->z);
  } else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}
````

```$xslt
NAN_SETTER(Vector::HandleSetters) {
  Vector* self = Nan::ObjectWrap::Unwrap<Vector>(info.This());

  if(!value->IsNumber()) {
    return Nan::ThrowError(Nan::New("expected value to be a number").ToLocalChecked());
  }

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "x") {
    self->x = value->NumberValue();
  } else if (propertyName == "y") {
    self->y = value->NumberValue();
  } else if (propertyName == "z") {
    self->z = value->NumberValue();
  }
}
```

You can also implement a getter and setter for each property seperately if you would like to. In this case you do not have to check for the property name as it is determined implicitly.

##### Build and run it! :
 ``
 npm install &&npm start
 ``
