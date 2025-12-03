#include <napi.h>
#include "hasher.h"

Napi::String HashWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    string pwd = info[0].As<Napi::String>().Utf8Value();
    string salt = info.Length() > 1 ? info[1].As<Napi::String>().Utf8Value() : "";
    int rounds = info.Length() > 2 ? info[2].As<Napi::Number>().Int32Value() : 3;
    size_t memKB = info.Length() > 3 ? (size_t)info[3].As<Napi::Number>().Int64Value() : 1024;
    if (salt.empty()) salt = generateSalt(16);
    string out = infiHash(pwd, salt, rounds, memKB);
    Napi::Object res = Napi::Object::New(env);
    res.Set("hash", Napi::String::New(env, out));
    res.Set("salt", Napi::String::New(env, salt));
    return res.Get("hash").As<Napi::String>();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("hash", Napi::Function::New(env, HashWrapped));
    return exports;
}

NODE_API_MODULE(infihash, Init)
