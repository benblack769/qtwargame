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

//this type is there to allow better safety and consistency
//for dealing with interations between RangeArrays and SquareIterates
struct BoardSquare{
    Point Cen;
    int Range;
    BoardSquare():Cen(0,0),Range(0){}
    BoardSquare(Point InCen, int InRange){
        Cen = InCen;
        Range = InRange;
    }
    //disallow equality to help ensure constness
    void operator =(BoardSquare & Other) = delete;
};
inline PIterContainter RectIterate(int xstart,int ystart,int xend,int yend){
    return PIterContainter(max(0,xstart),max(0,ystart),min(BoardSizeX,xend),min(BoardSizeY,yend));
}
inline PIterContainter BoardIterate(){
    return PIterContainter(0,0,BoardSizeX,BoardSizeY);
}
inline PIterContainter SquareIterate(Point Cen,int Range){
    return RectIterate(Cen.X-Range,Cen.Y-Range,Cen.X+Range+1,Cen.Y+Range+1);
}
inline PIterContainter SquareIterate(BoardSquare S){
    return SquareIterate(S.Cen,S.Range);
}
inline PIterContainter RectIterateInclusive(int xstart,int ystart,int xend,int yend){
    return RectIterate(xstart,ystart,xend+1,yend+1);
}
