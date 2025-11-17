#ifndef MHASHTABLE_HH
#define MHASHTABLE_HH

#include "hash.hh"
#include "hashnode.hh"

const double MAX_LOAD_FACTOR = 0.75;
const size_t H_INCREMENT = 20;

template <typename K, typename V>
class MhashTable{
public:
    MhashTable();
    MhashTable(size_t inCapacity);
    MhashTable(const MhashTable& inMhashTable);
    ~MhashTable();
    size_t getHash(const K& inKey);
    void resize(size_t newCapacity);
    size_t findSlot(const K& inKey);
    MhashTable& operator=(const MhashTable& inMhashTable);
    void insert(const K& inKey, const V& inValue);
    V& operator[](const K& inKey);
    bool find(const K& inKey);
    V* get(const K& inKey);
    size_t size();
    bool empty();
    void clear();

    HashNode<K, V>* table;
    size_t capacity;
    size_t length;
    HashFNV1a hashFunction;
};

template <typename K, typename V>
MhashTable<K, V>::MhashTable(){
    capacity = H_INCREMENT;
    length = 0;
    table = new HashNode<K, V>[capacity];
}

template <typename K, typename V>
MhashTable<K, V>::MhashTable(size_t inCapacity){
    capacity = inCapacity;
    length = 0;
    table = new HashNode<K, V>[capacity];
}

template <typename K, typename V>
MhashTable<K, V>::MhashTable(const MhashTable& inMhashTable){
    capacity = inMhashTable.capacity;
    length = inMhashTable.length;
    table = new HashNode<K, V>[capacity];
    for(size_t i = 0; i < capacity; i++)
        table[i] = inMhashTable.table[i];
}

template <typename K, typename V>
MhashTable<K, V>::~MhashTable(){
    delete[] table;
}

template <typename K, typename V>
size_t MhashTable<K, V>::getHash(const K& inKey){
    return hashFunction(inKey) % capacity;
}

template <typename K, typename V>
void MhashTable<K, V>::resize(size_t newCapacity){
    HashNode<K, V>* oldTable = table;
    size_t oldCapacity = capacity;
    table = new HashNode<K, V>[newCapacity];
    capacity = newCapacity;
    length = 0;
    for(size_t i = 0; i < oldCapacity; i++){
        if(oldTable[i].used)
            insert(oldTable[i].key, oldTable[i].value);
    }
    delete[] oldTable;
}

template <typename K, typename V>
size_t MhashTable<K, V>::findSlot(const K& inKey){
    size_t idx = getHash(inKey);
    size_t start = idx;
    while(table[idx].used){
        if(table[idx].key == inKey)
            return idx;
        idx = (idx + 1) % capacity;
        if(idx == start)
            break;
    }
    return idx;
}

template <typename K, typename V>
MhashTable<K, V>& MhashTable<K, V>::operator=(const MhashTable& inMhashTable){
    if(this != &inMhashTable){
        delete[] table;
        capacity = inMhashTable.capacity;
        length = inMhashTable.length;
        table = new HashNode<K, V>[capacity];
        for(size_t i = 0; i < capacity; i++)
            table[i] = inMhashTable.table[i];
    }
    return *this;
}

template <typename K, typename V>
void MhashTable<K, V>::insert(const K& inKey, const V& inValue){
    if(static_cast<double>(length + 1) / capacity > MAX_LOAD_FACTOR)
        resize(capacity + H_INCREMENT);
    size_t idx = findSlot(inKey);
    if(table[idx].used && table[idx].key == inKey)
        table[idx].value = inValue;
    else{
        table[idx] = HashNode<K, V>(inKey, inValue);
        length++;
    }
}

template <typename K, typename V>
V& MhashTable<K, V>::operator[](const K& inKey) {
    size_t idx = findSlot(inKey);
    if(!table[idx].used){
        insert(inKey, V());
        idx = findSlot(inKey);
    }
    return table[idx].value;
}

template <typename K, typename V>
bool MhashTable<K, V>::find(const K& inKey){
    size_t idx = getHash(inKey);
    size_t start = idx;
    while(table[idx].used){
        if(table[idx].key == inKey)
            return true;
        idx = (idx + 1) % capacity;
        if(idx == start)
            break;
    }
    return false;
}

template <typename K, typename V>
V* MhashTable<K, V>::get(const K& inKey) {
    size_t idx = getHash(inKey);
    size_t start = idx;
    while(table[idx].used) {
        if(table[idx].key == inKey)
            return &table[idx].value;
        idx = (idx + 1) % capacity;
        if(idx == start)
            break;
    }
    return nullptr;
}

template <typename K, typename V>
size_t MhashTable<K, V>::size(){
    return length;
}

template <typename K, typename V>
bool MhashTable<K, V>::empty(){
    return !length;
}

template <typename K, typename V>
void MhashTable<K, V>::clear() {
    delete[] table;
    table = new HashNode<K, V>[capacity];
    length = 0;
}

#endif
