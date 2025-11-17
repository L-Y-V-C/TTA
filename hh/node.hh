#ifndef NODE_HH
#define NODE_HH

#include "entry.hh"
#include "mvector.hh"

const int MAX_ENTRIES = 10;
const int MIN_ENTRIES = 1;

class Node{
public:
    Node(bool inIsLeaf = true) : isLeaf(inIsLeaf){
        entries.reserve(MAX_ENTRIES + 1);
    }

    bool isLeaf;
    Mvector<Entry> entries;
};

#endif
