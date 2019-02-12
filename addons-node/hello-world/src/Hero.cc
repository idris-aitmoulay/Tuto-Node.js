#include "Hero.h"

Nan::Persistent<v8::FunctionTemplate> Hero::constructor;

NAN_MODULE_INIT(Hero::Init) {
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Hero::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Hero").ToLocalChecked());

  // link our getters and setter to the object property
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("life").ToLocalChecked(), Hero::HandleGetters, Hero::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("username").ToLocalChecked(), Hero::HandleGetters, Hero::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("age").ToLocalChecked(), Hero::HandleGetters, Hero::HandleSetters);

  target->Set(Nan::New("Hero").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(Hero::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("Hero::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if(info.Length() != 3) {
    return Nan::ThrowError(Nan::New("Hero::New - expected arguments life, age, username").ToLocalChecked());
  }

  // expect arguments to be numbers
  if(!info[0]->IsNumber() || !info[1]->IsString() || !info[2]->IsNumber()) {
    return Nan::ThrowError(Nan::New("Hero::New - expected arguments to be numbers").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance
  Hero* hero = new Hero();
  hero->Wrap(info.Holder());

  // initialize it's values
  hero->life = info[0]->NumberValue();
  hero->username = info[1]->SameValue();
  hero->age = info[2]->NumberValue();

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(Hero::HandleGetters) {
  Hero* self = Nan::ObjectWrap::Unwrap<Hero>(info.This());

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "life") {
    info.GetReturnValue().Set(self->life);
  } else if (propertyName == "age") {
    info.GetReturnValue().Set(self->age);
  } else if (propertyName == "username") {
    info.GetReturnValue().Set(self->username);
  } else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}


NAN_SETTER(Hero::HandleSetters) {
  Hero* self = Nan::ObjectWrap::Unwrap<Hero>(info.This());

  if(!value->IsNumber() || !value->IsString()) {
    return Nan::ThrowError(Nan::New("expected value to be a number or string").ToLocalChecked());
  }

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "age") {
    self->age = value->NumberValue();
  } else if (propertyName == "life") {
    self->life = value->NumberValue();
  } else if (propertyName == "username") {
    self->username = value->SameValue();
  }
}
