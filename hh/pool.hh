#ifndef POOL_HH
#define POOL_HH

#include "node.hh"
#include "mvector.hh"

class Pool{
public:
    Pool();
    int assignNode(bool inIsLeaf);
    Node& operator[](int idx);
    size_t size();
    void clear();

    Mvector<Node> nodes;
};

#endif
