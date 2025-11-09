#include <napi.h>
#include "hasher.h"

Napi::String HashWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    string input = info[0].As<Napi::String>().Utf8Value();
    string salt = info[1].As<Napi::String>().Utf8Value();
    int rounds = info[2].As<Napi::Number>().Int32Value();

    string result = customHash(input, salt, rounds);
    return Napi::String::New(env, result);
}

Napi::String GenerateSaltWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int length = info[0].As<Napi::Number>().Int32Value();
    string salt = generateSalt(length);
    return Napi::String::New(env, salt);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("hash", Napi::Function::New(env, HashWrapped));
    exports.Set("generateSalt", Napi::Function::New(env, GenerateSaltWrapped));
    return exports;
}

NODE_API_MODULE(infihash, Init)
