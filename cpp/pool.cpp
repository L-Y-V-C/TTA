#include "../hh/pool.hh"

Pool::Pool(){
    nodes.reserve(100);
}

int Pool::assignNode(bool inIsLeaf){
    nodes.push_back(Node(inIsLeaf));
    return nodes.size() - 1;
}

Node& Pool::operator[](int idx){
    return nodes[idx];
}

size_t Pool::size(){
    return nodes.size();
}

void Pool::clear(){
    nodes.clear();
}
