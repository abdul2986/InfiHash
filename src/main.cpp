#include <napi.h>
#include "hasher.h"

std::string encodeHash(const std::string &salt, const std::string &hash)
{
    return salt + ":" + hash;
}

std::pair<std::string, std::string> decodeHash(const std::string &stored)
{
    auto pos = stored.find(':');
    if (pos == std::string::npos)
        return {"", ""};
    return {stored.substr(0, pos), stored.substr(pos + 1)};
}

Napi::String HashWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string pwd = info[0].As<Napi::String>().Utf8Value();
    std::string salt = info.Length() > 1 && !info[1].As<Napi::String>().Utf8Value().empty()
                           ? info[1].As<Napi::String>().Utf8Value()
                           : generateSalt(16);
    int rounds = info.Length() > 2 ? info[2].As<Napi::Number>().Int32Value() : 3;
    size_t memKB = info.Length() > 3 ? (size_t)info[3].As<Napi::Number>().Int64Value() : 1024;

    std::string hashed = infiHash(pwd, salt, rounds, memKB);
    std::string stored = encodeHash(salt, hashed);

    return Napi::String::New(env, stored);
}

Napi::Boolean VerifyWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string pwd = info[0].As<Napi::String>().Utf8Value();
    std::string storedHash = info[1].As<Napi::String>().Utf8Value();
    int rounds = info.Length() > 2 ? info[2].As<Napi::Number>().Int32Value() : 3;
    size_t memKB = info.Length() > 3 ? (size_t)info[3].As<Napi::Number>().Int64Value() : 1024;

    auto [salt, hash] = decodeHash(storedHash);
    if (salt.empty() || hash.empty())
        return Napi::Boolean::New(env, false);

    std::string hashedInput = infiHash(pwd, salt, rounds, memKB);
    return Napi::Boolean::New(env, hashedInput == hash);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hash", Napi::Function::New(env, HashWrapped));
    exports.Set("verify", Napi::Function::New(env, VerifyWrapped));
    return exports;
}

NODE_API_MODULE(infihash, Init)
