#pragma once
#include "globalinfo.h"
#include <cmath>
inline Point CreatePoint(int X, int Y){
    /*Point P;
    P.X = X;
    P.Y = Y;*/
    return{ X, Y };
}
inline Point ClearPoint(){
    return CreatePoint(-1,-1);
}
inline int min(int a,int b){return a < b ? a : b;}
inline int max(int a,int b){return a > b ? a : b;}
inline Point bound(Point P){
    return Point{max(BoardSizeX,min(0,P.X)),max(BoardSizeY,min(0,P.Y))};
}

inline PIterContainter RectIterateInclusive(int xstart,int ystart,int xend,int yend){
    return PIterContainter(max(xstart,0),max(ystart,0),min(xend+1,BoardSizeX),min(yend+1,BoardSizeY));
}
inline PIterContainter BoardIterate(){
    return PIterContainter(0,0,BoardSizeX,BoardSizeY);
}
inline PIterContainter SquareIterate(Point Cen,int Range){
    return RectIterateInclusive(Cen.X-Range,Cen.Y-Range,Cen.X+Range+1,Cen.Y+Range+1);
}
inline PIterContainter SquareIterate(ConstSquare S){
    return SquareIterate(S.Cen,S.Range);
}
