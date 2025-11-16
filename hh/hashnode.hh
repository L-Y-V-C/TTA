#ifndef HASHNODE_HH
#define HASHNODE_HH

template <typename K, typename V>
class HashNode{
public:
    HashNode() : used(false){}
    HashNode(const K& inKey, const V& inValue) :
        key(inKey), value(inValue), used(true){}

    K key;
    V value;
    bool used;
};

#endif
