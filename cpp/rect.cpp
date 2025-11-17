#include "../hh/rect.hh"

Rect::Rect(int x, int y){
    minX = x;
    minY = y;
    maxX = x;
    maxY = y;
}

Rect::Rect(int inMinX, int inMinY, int inMaxX, int inMaxY){
    minX = inMinX;
    minY = inMinY;
    maxX = inMaxX;
    maxY = inMaxY;
}

bool Rect::overlaps(const Rect& inRect){
    return !(minX > inRect.maxX || maxX < inRect.minX ||
             minY > inRect.maxY || maxY < inRect.minY);
}

Rect Rect::combine(const Rect& inRect){
    return Rect(min(minX, inRect.minX),
                min(minY, inRect.minY),
                max(maxX, inRect.maxX),
                max(maxY, inRect.maxY));
}

int Rect::area(){
    return (maxX - minX) * (maxY - minY);
}
