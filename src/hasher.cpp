#include "hasher.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <queue>
#include <stack>
#include <deque>
#include <unordered_map>
#include <cstring>
#include <mutex>

using namespace std;

string generateSalt(int length) {
    static const char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const int charsLen = 62;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int> dist(0, charsLen - 1);
    string s;
    s.reserve(length);
    for (int i = 0; i < length; ++i) s.push_back(chars[dist(gen)]);
    return s;
}

static inline uint64_t rotl64(uint64_t x, unsigned r) {
    return (x << r) | (x >> (64 - r));
}

static inline uint64_t mix64(uint64_t a, uint64_t b, uint64_t c) {
    a ^= b; a = rotl64(a, 27);
    b ^= c; b = rotl64(b, 31);
    c ^= a; c = c * 0x9ddfea08eb382d69ULL + 0x9e3779b97f4a7c15ULL;
    return a ^ b ^ c;
}

string infiHash(const string& password, const string& salt, int rounds, size_t memKB) {
    size_t totalBytes = memKB * 1024;
    if (totalBytes < 64) totalBytes = 64;
    size_t lanes = 4;
    size_t blockSize = 64;
    size_t blockCount = totalBytes / blockSize;
    if (blockCount < lanes * 2) blockCount = lanes * 2;
    uint64_t seed = 0xcbf29ce484222325ULL;
    for (char c : password) seed = (seed ^ (uint64_t)(unsigned char)c) * 0x100000001b3ULL;
    for (char c : salt) seed = (seed ^ (uint64_t)(unsigned char)c) * 0x100000001b3ULL;

    uint64_t* mem = new uint64_t[blockCount * (blockSize / sizeof(uint64_t))];
    size_t wordsPerBlock = blockSize / sizeof(uint64_t);

    mt19937_64 prng((uint64_t)seed ^ 0x9e3779b97f4a7c15ULL);
    for (size_t i = 0; i < blockCount * wordsPerBlock; ++i) mem[i] = prng();

    deque<string> recent;
    queue<uint64_t> q;
    stack<uint64_t> st;
    unordered_map<uint64_t, uint64_t> mapIndex;
    for (size_t i = 0; i < blockCount; ++i) mapIndex[(uint64_t)i ^ seed] = (uint64_t)i;

    for (int r = 0; r < rounds; ++r) {
        for (size_t lane = 0; lane < lanes; ++lane) {
            size_t start = lane * (blockCount / lanes);
            size_t end = start + (blockCount / lanes);
            for (size_t b = start; b < end; ++b) {
                uint64_t* block = mem + b * wordsPerBlock;
                uint64_t a = block[0] ^ (uint64_t)password.size() ^ ((uint64_t)salt.size() << 32) ^ r;
                for (size_t w = 0; w < wordsPerBlock; ++w) {
                    a = mix64(a, block[w], (uint64_t)b + w + seed);
                    block[w] ^= a;
                    q.push(block[w] ^ ((uint64_t)b + w));
                    if (q.size() > 64) {
                        uint64_t v = q.front(); q.pop();
                        st.push(v ^ rotl64(a, 13));
                    }
                }
                while (!st.empty()) {
                    uint64_t v = st.top(); st.pop();
                    size_t idx = (v ^ seed) % blockCount;
                    uint64_t* target = mem + idx * wordsPerBlock;
                    for (size_t w = 0; w < wordsPerBlock; ++w) target[w] = mix64(target[w], v, a);
                }
            }
        }

        for (size_t i = 0; i < blockCount; ++i) {
            uint64_t acc = 0x9e3779b97f4a7c15ULL;
            uint64_t* block = mem + i * wordsPerBlock;
            for (size_t w = 0; w < wordsPerBlock; ++w) acc = mix64(acc, block[w], (uint64_t)w + i);
            uint64_t key = acc ^ ((uint64_t)rounds << 32) ^ seed;
            recent.push_back(to_string(key));
            if (recent.size() > 128) recent.pop_front();
        }

        for (size_t i = 0; i < blockCount; ++i) {
            uint64_t* bptr = mem + i * wordsPerBlock;
            for (size_t w = 0; w < wordsPerBlock; ++w) bptr[w] ^= (uint64_t)(i ^ r) + rotl64(bptr[(w + 7) % wordsPerBlock], 17);
        }
    }

    uint64_t out1 = 0x0123456789abcdefULL;
    uint64_t out2 = 0xfedcba9876543210ULL;
    uint64_t out3 = seed ^ (uint64_t)password.size();
    uint64_t out4 = seed ^ (uint64_t)salt.size();

    for (size_t i = 0; i < blockCount; ++i) {
        uint64_t* block = mem + i * wordsPerBlock;
        out1 = mix64(out1, block[0], block[wordsPerBlock - 1]);
        out2 = mix64(out2, block[1 % wordsPerBlock], block[(wordsPerBlock / 2) % wordsPerBlock]);
        out3 = mix64(out3, block[2 % wordsPerBlock], block[(wordsPerBlock / 3) % wordsPerBlock]);
        out4 = mix64(out4, block[3 % wordsPerBlock], block[(wordsPerBlock / 5) % wordsPerBlock]);
    }

    uint8_t finalBytes[32];
    memcpy(finalBytes + 0, &out1, 8);
    memcpy(finalBytes + 8, &out2, 8);
    memcpy(finalBytes + 16, &out3, 8);
    memcpy(finalBytes + 24, &out4, 8);

    std::ostringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < 32; ++i) ss << setw(2) << (int)(finalBytes[i] & 0xff);

    memset(mem, 0, blockCount * wordsPerBlock * sizeof(uint64_t));
    delete[] mem;

    return ss.str();
}
