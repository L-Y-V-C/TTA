#ifndef HASH_HH
#define HASH_HH

#include <string>

using namespace std;

class HashFNV1a{
public:
    size_t operator()(const string& key) const{
        size_t fnvPrime = 0x00000100000001B3ULL;
        size_t fnvOffsetBasis = 0xCBF29CE484222325ULL;
        size_t h = fnvOffsetBasis;
        for(char c : key){
            h ^= static_cast<size_t>(c);
            h *= fnvPrime;
        }
        return h;
    }
    size_t operator()(const int& key){
        size_t fnvPrime = 0x00000100000001B3ULL;
        size_t offset = 0xCBF29CE484222325ULL;
        size_t h = offset;
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&key);
        for(size_t i = 0; i < sizeof(key); i++){
            h ^= static_cast<size_t>(bytes[i]);
            h *= fnvPrime;
        }
        return h;
    }
};

#endif
