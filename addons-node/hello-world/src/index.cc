#include <nan.h>
#include "Hero.h"

NAN_MODULE_INIT(InitModule) {
  Hero::Init(target);
}

NODE_MODULE(myModule, InitModule);
