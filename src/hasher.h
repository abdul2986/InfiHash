#ifndef HASHER_H
#define HASHER_H

#include <string>

using namespace std;

string generateSalt(int length);
string infiHash(const string &password, const string &salt, int rounds, size_t memKB);

#endif