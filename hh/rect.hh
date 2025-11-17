#ifndef RECT_HH
#define RECT_HH

#include <algorithm>

using namespace std;

class Rect{
public:
    Rect(int x = 0, int y = 0);
    Rect(int inMinX, int inMinY, int inMaxX, int inMaxY);
    bool overlaps(const Rect& inRect);
    Rect combine(const Rect& inRect);
    int area();

    int minX, minY, maxX, maxY;
};

#endif
