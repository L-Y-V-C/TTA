#ifndef ENTRY_HH
#define ENTRY_HH

#include "../hh/rect.hh"

class Entry{
public:
    Entry() : childIdx(0), x(0), y(0), topicId(0){}
    Entry(int inX, int inY, int inTopicId) : // for leaf
        rect(inX, inY), childIdx(0), x(inX), y(inY), topicId(inTopicId){}
    Entry(const Rect& inRect, int inChildIdx) : // for intern
        rect(inRect), childIdx(inChildIdx), x(0), y(0), topicId(0){}

    Rect rect;
    int childIdx, x, y, topicId;
};

#endif
