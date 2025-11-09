#include "hasher.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

string generateSalt(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string salt = "";
    srand(time(0));
    for (int i = 0; i < length; i++) {
        salt += chars[rand() % chars.size()];
    }
    return salt;
}

string customHash(string input, string salt, int rounds) {
    string data = input + salt;
    unsigned long long hash = 5381;
    vector<int> chain;

    for (char c : data) chain.push_back((int)c);

    for (int r = 0; r < rounds; r++) {
        unsigned long long mix = 0;
        for (int val : chain) {
            hash = ((hash << 5) + hash) + val; 
            mix ^= (hash >> (r % 7)) ^ (val * (r + 13));
        }
        hash ^= (mix + r * 97);
        hash = (hash * 2654435761) % 4294967291; 
        chain.push_back(hash % 256);
    }

    stringstream ss;
    ss << hex << hash;
    return ss.str();
}
