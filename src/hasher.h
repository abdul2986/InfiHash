#ifndef HASHER_H
#define HASHER_H

#include <string>
using namespace std;

string generateSalt(int length);
string customHash(string input, string salt, int rounds);

#endif
