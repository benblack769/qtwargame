#pragma once
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <algorithm>

struct Point{
    int32_t X;
    int32_t Y;
    Point():X(0),Y(0){}
    Point(int32_t inX,int32_t inY):X(inX),Y(inY){}
};
inline Point abs(Point P){
    return Point(abs(P.X), abs(P.Y));
}
inline bool operator==(Point P1, Point P2){
    return P1.X == P2.X && P1.Y == P2.Y;
}
inline bool operator!=(Point P1, Point P2){
    return !(P1 == P2);
}
inline void operator += (Point & P1, Point P2){//refrence first
    P1.X += P2.X;
    P1.Y += P2.Y;
}
inline Point operator + (Point P1, Point P2){//do not make Point a refrence!!!
    //P1 is copied, so the += do not affect the original
    P1 += P2;
    return P1;
}
inline Point operator - (Point P){//do not refrence!
    P.X = -P.X;
    P.Y = -P.Y;
    return P;
}
inline void operator -= (Point & P1, Point P2){//refrence first!
    P1.X -= P2.X;
    P1.Y -= P2.Y;
}
inline Point operator - (Point P1, Point P2){//do not refrence!
    P1 -= P2;
    return P1;
}
template <typename NumType>
inline void operator *= (Point & P1, NumType Num){//refrence first
    P1.X *= Num;
    P1.Y *= Num;
}
template <typename NumType>
inline void operator /= (Point & P1, NumType Num){//refrence first
    P1.X /= Num;
    P1.Y /= Num;
}
template <typename NumType>
inline Point operator * (Point P1, NumType Mult){
    P1 *= Mult;
    return P1;
}
template <typename NumType>
inline Point operator / (Point P1, NumType Mult){
    P1 /= Mult;
    return P1;
}
template<int32_t MaxX,int32_t MaxY>
class PointIter{
public:
    PointIter(int xstart, int ystart, int xend, int yend){
        XCap = std::min(xend,MaxX);
        YCap = std::min(yend,MaxY);

        P.X = std::max(xstart,0);
        P.Y = std::max(ystart,0);

        YLow = P.Y;
    }
    PointIter(Point Center, int Range):
        PointIter(Center.X - Range,
                      Center.Y - Range,
                      Center.X + Range + 1,
                      Center.Y + Range + 1){}
    PointIter():
        PointIter(0,0,MaxX,MaxY){}

    bool NotEnd(){
        return P.X < XCap;
    }
    bool operator != (PointIter & Other){
        return P.X < Other.XCap;
    }

    void operator++ (){
        P.Y++;
        if (P.Y >= YCap){
            P.X++;
            P.Y = YLow;
        }
    }
    Point operator * (){
        return P;
    }
private:
    Point P;
    int XCap,YCap,YLow;
};

template<int32_t MaxX,int32_t MaxY>
class PIterContainter
{
public:
    PointIter<MaxX,MaxY> EndIter,StartIter;
    PIterContainter(Point Center, int Range){
        EndIter = PointIter<MaxX,MaxY>(Center, Range);
        StartIter = EndIter;
    }
    PIterContainter(int xstart, int ystart, int xend, int yend){
        EndIter = PointIter<MaxX,MaxY>(xstart, ystart, xend, yend);
        StartIter = EndIter;
    }
    PIterContainter(){
        EndIter = PointIter<MaxX,MaxY>();
        StartIter = EndIter;
    }
    PointIter<MaxX,MaxY> & begin(){
        return StartIter;
    }
    PointIter<MaxX,MaxY> & end(){
        return EndIter;
    }
};

template<int32_t MaxX,int32_t MaxY>
inline PIterContainter<MaxX,MaxY> SquareIterOver(Point Cen,int Range){
    return PIterContainter<MaxX,MaxY>(Cen,Range);
}

template<int32_t MaxX,int32_t MaxY>
inline PIterContainter<MaxX,MaxY> RectIterOver(int xstart,int ystart,int xend,int yend){
    return PIterContainter<MaxX,MaxY>(xstart,ystart,xend,yend);
}
