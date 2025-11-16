#ifndef RECT_HH
#define RECT_HH

#include <algorithm>

using namespace std;

class Rect{
public:
    Rect(int x = 0, int y = 0) :
        minX(x), minY(y), maxX(x), maxY(y){}
    Rect(int inMinX, int inMinY, int inMaxX, int inMaxY) :
        minX(inMinX), minY(inMinY), maxX(inMaxX), maxY(inMaxY){}
    bool overlaps(const Rect& inRect){
        return !(minX > inRect.maxX || maxX < inRect.minX ||
                 minY > inRect.maxY || maxY < inRect.minY);
    }
    Rect combine(const Rect& inRect){
        return Rect(min(minX, inRect.minX),
                    min(minY, inRect.minY),
                    max(maxX, inRect.maxX),
                    max(maxY, inRect.maxY));
    }
    int area(){
        return (maxX - minX) * (maxY - minY);
    }

    int minX, minY, maxX, maxY;
};

#endif
