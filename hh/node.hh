#ifndef NODE_HH
#define NODE_HH

#include <cmath>

#include "entry.hh"
#include "mvector.hh"

const int MAX_ENTRIES = 5;
const int MIN_ENTRIES = ceil(MAX_ENTRIES * 0.4);

class Node{
public:
    Node(bool inIsLeaf = true) : isLeaf(inIsLeaf){
        entries.reserve(MAX_ENTRIES + 1);
    }

    bool isLeaf;
    Mvector<Entry> entries;
};

#endif
