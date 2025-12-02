#include "hasher.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <queue>
#include <stack>

struct SaltNode
{
    char val;
    SaltNode *next;
    SaltNode(char c) : val(c), next(nullptr) {}
};

string generateSalt(int length)
{
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string salt = "";
    srand(time(0));
    for (int i = 0; i < length; i++)
        salt += chars[rand() % chars.size()];
    return salt;
}

unsigned long long treeMix(unsigned long long h)
{
    unsigned long long a = (h ^ (h >> 7));
    unsigned long long b = (h ^ (h << 11));
    unsigned long long c = (a ^ b) * 11400714819323198485ull;
    return c ^ (c >> 13);
}

string customHash(string input, string salt, int rounds)
{
    // --- Build linked list salt ---
    SaltNode *head = nullptr;
    for (int i = salt.size() - 1; i >= 0; i--)
    {
        SaltNode *node = new SaltNode(salt[i]);
        node->next = head;
        head = node;
    }

    // Queue + Stack for chaining
    queue<int> q;
    stack<int> st;

    for (char c : input)
        q.push((int)c);

    // Add salt to queue
    SaltNode *temp = head;
    while (temp)
    {
        q.push((int)temp->val);
        temp = temp->next;
    }

    unsigned long long hash = 146527;
    unsigned long long primes[] = {97, 193, 389, 769, 1543, 3079};

    // --- Adaptive load balancing ---
    int adaptive = (input.size() + salt.size()) % 5;
    rounds += adaptive;

    for (int r = 0; r < rounds; r++)
    {

        // 1) Move queue → stack (forward influence)
        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            hash = (hash * 131) ^ (v + r * 17);
            st.push(v ^ (hash >> 5));
        }

        // 2) Stack → queue (reverse compression)
        while (!st.empty())
        {
            int v = st.top();
            st.pop();
            hash ^= (v * (r + 11));
            q.push(v ^ (hash << 3));
        }

        // 3) Prime modular shuffling
        hash = (hash * primes[r % 6]) % 4294967291ull;

        // 4) Bitwise tree transformation
        hash = treeMix(hash);
    }

    stringstream ss;
    ss << hex << hash;
    return ss.str();
}
