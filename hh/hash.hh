#ifndef HASH_HH
#define HASH_HH

#include <string>

using namespace std;

class HashFNV1a{
public:
    size_t operator()(const string& key){
        size_t fnvPrime = 0x00000100000001B3ULL;
        size_t fnvOffsetBasis = 0xCBF29CE484222325ULL;
        size_t h = fnvOffsetBasis;
        for(char c : key){
            h ^= static_cast<size_t>(c);
            h *= fnvPrime;
        }
        return h;
    }
};

#endif
