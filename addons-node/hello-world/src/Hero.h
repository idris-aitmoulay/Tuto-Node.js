#include <nan.h>
#include <string>

using namespace std;

class Hero : public Nan::ObjectWrap {
public:
    double life;
    string username;
    double age;

    /**
    * it's an Init function to initialize instance of this class.
    */
    static NAN_MODULE_INIT(Init);

    static NAN_METHOD(New);

    /**
    * getter & setter function
    */
    static NAN_GETTER(HandleGetters);
    static NAN_SETTER(HandleSetters);

    static Nan::Persistent<v8::FunctionTemplate> constructor;
};
