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
using PItContainter = PIterContainter<BoardSizeX,BoardSizeY>;
using PointIterator = PointIter<BoardSizeX,BoardSizeY>;

inline PItContainter BoardIterate(){
    return PItContainter(0,0,BoardSizeX,BoardSizeY);
}
inline PItContainter SquareIterate(BoardSquare S){
    return PItContainter(S.Cen,S.Range);
}
inline PItContainter SquareIterate(Point Cen,int Range){
    return PItContainter(Cen,Range);
}
inline PItContainter RectIterateInclusive(int xstart,int ystart,int xend,int yend){
    return PItContainter(xstart,ystart,xend+1,yend+1);
}
