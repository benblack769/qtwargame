#pragma once
#include "globalinfo.h"
#include <vector>
#include <cmath>
inline int RoundUpRatio(int Num, unsigned int Denom){
    return (Num + Denom - 1) / Denom;
}
template<typename Type>
inline Type Square(Type Num){
    return Num * Num;
}
template<typename NumTy>
inline NumTy Cube(NumTy Num){
    return Num * Num * Num;
}
inline bool IsBetween(double Val, double LowerBound, double UpperBound){
    return Val >= LowerBound && Val <= UpperBound;
}
inline double DisConvert(int Dis){
    return 1.0 / (1 + Dis);
}
#define BlankSpot(xspot,yspot)  (PlayerOcc[yspot][xspot] < 0)
#define BlankPoint(PPoint)  (PlayerOcc[PPoint] < 0)
#define NullPoint(PPoint) (PPoint.X < 0 || PPoint.Y < 0)

inline bool OnBoard(Point P){
    return P.X < BoardSizeX && P.Y < BoardSizeY && P.X >= 0 && P.Y >= 0;
}
template<typename ThisSet>
inline void InsertSet(ThisSet & Into,const ThisSet & From){
    Into.insert(From.begin(),From.end());
}
inline int GetBoardDistance(Point P1,Point P2){
    return max(abs(P1.X - P2.X),abs(P1.Y - P2.Y));
}
inline double TrueDistance(Point P1,Point P2){
    return sqrt(Square(P1.X - P2.X)+Square(P1.Y - P2.Y));
}
inline int GetBoardDistance(int x1,int y1,int x2,int y2){
    return max(abs(x1 - x2), abs(y1 - y2));
}
template<typename Ty>
Ty max(const vector<Ty> & Vec){
    auto MaxVal = Vec.front();
    for (auto & Num : Vec)
        if (MaxVal < Num)
            MaxVal = Num;
    return MaxVal;
}
template<typename Ty>
Ty min(const vector<Ty> & Vec){
    auto MinVal = Vec.front();
    for (auto & Num : Vec)
        if (MinVal > Num)
            MinVal = Num;
    return MinVal;
}
template <typename T>
int sign(T val) {
    return (T(0) < val) - (val < T(0));
}
template<typename Ty>
Ty sum(const vector<Ty> & Vec){
    if (Vec.size() == 0)
        return Ty();
    auto Sum = Vec.front();
    for_each(Vec.begin() + 1, Vec.end(), [&](decltype(Vec.front()) Val){
        Sum += Val;
    });
    return Sum;
}
template<typename VecType, typename PointerType>
inline int PointerToNum(const VecType & V,PointerType Ptr){
    /*
     * V is a vector of unique values.
     * Ptr is a value in that vector
     * returns integer index to that value
    */
    int Size = V.size();
    for (int g = 0; g < Size; g++){
        if (V[g] == Ptr)
            return g;
    }
}
template<typename MapType>//the map must have the value that needs to maximized as the store type
inline auto MaxOf(const MapType & ThisMap) -> decltype(ThisMap.begin()){
    using itty = decltype(ThisMap.begin());
    itty MaxIt = ThisMap.begin();
    for (itty It = ThisMap.begin(); It != ThisMap.end(); It++){
        if (It->second > MaxIt->second){
            MaxIt = It;
        }
    }
    return MaxIt;
}

template<typename AssociateType>
struct ValInfo{
    ValInfo(double StartVal, AssociateType InInfo):Val(StartVal),Info(InInfo){}
    ValInfo():Val(-1e50),Info( AssociateType()){}
    bool operator < (const ValInfo & Other){
        return this->Val < Other.Val;
    }
    bool operator > (const ValInfo & Other){
        return this->Val > Other.Val;
    }
    bool operator == (const ValInfo & Other){
        return this->Val == Other.Val;
    }
    bool operator != (const ValInfo & Other){
        return !(*this == Other);
    }
    double Val;
    AssociateType Info;
};
using PointVal = ValInfo<Point>;

template<typename ty1,typename ty2,typename ty3>
struct triple{
    ty1 first = ty1();
    ty2 second = ty2();
    ty3 third = ty3();
    triple():first(ty1()),second(ty2()),third(ty3()){}
    triple(ty1 f,ty2 s,ty3 t):first(f),second(s),third(t){}
};
