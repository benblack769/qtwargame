#pragma once
#include <algorithm>
#include "thingstuff.h"
template<typename FuncType>
void FastSquareIter(Point CenP,int Range,FuncType Func){
    try{
        Point OutP;
        int MaxX = min(CenP.X + Range, BoardSizeX - 1);
        int MinX = max(CenP.X - Range, 0);
        int MaxY = min(CenP.Y + Range, BoardSizeY - 1);
        int MinY = max(CenP.Y - Range, 0);

        for (OutP.X = MinX;
            OutP.X <= MaxX;
            OutP.X++){
            for (OutP.Y = MinY;
                OutP.Y <= MaxY;
                OutP.Y++){
                Func(OutP);
            }
        }
    }
    catch (...){}
}
template<typename FuncType>
void FastCoordSquareIter(Point CenP, int Range, FuncType Func){
    try{
        for (int X = max(CenP.X - Range, 0);
            X <= min(CenP.X + Range, BoardSizeX - 1);
            X++){
            for (int Y = max(CenP.Y - Range, 0);
                Y <= min(CenP.Y + Range, BoardSizeY - 1);
                Y++){
                Func(X,Y);
            }
        }
    }
    catch (...){}
}
template<typename FuncType>
void FastExpandingIter(Point CenP,int StartRange,int MaxRange, FuncType Express){
    Point OutP;
    try{
        if (StartRange == 0 && MaxRange > 0)//special case of zero needs this attention
            Express(CenP);
        for (int Range = StartRange; Range < MaxRange; Range++){
            OutP.Y = CenP.Y - Range;
            if (OutP.Y >= 0){
                for (OutP.X = max(CenP.X - Range, 0); OutP.X < min(CenP.X + Range, BoardSizeX - 1); OutP.X++){
                    Express(OutP);
                }
            }
            OutP.Y = CenP.Y + Range;
            if (OutP.Y < BoardSizeY){
                for (OutP.X = min(CenP.X + Range, BoardSizeX - 1); OutP.X > max(CenP.X - Range, 0); OutP.X--){
                    Express(OutP);
                }
            }
            OutP.X = CenP.X - Range;
            if (OutP.X >= 0){
                for (OutP.Y = max(CenP.Y - Range, 0); OutP.Y < min(CenP.Y + Range, BoardSizeY - 1); OutP.Y++){
                    Express(OutP);
                }
            }
            OutP.X = CenP.X + Range;
            if (OutP.X < BoardSizeX){
                for (OutP.Y = min(CenP.Y + Range, BoardSizeY - 1); OutP.Y > max(CenP.Y - Range, 0); OutP.Y--){
                    Express(OutP);
                }
            }
        }
    }
    catch (...){}
}
template<typename FuncType>
void FastBoardIter(FuncType Func){
    Point P;
    try{
        for (P.X = 0; P.X < BoardSizeX; P.X++){
            for (P.Y = 0; P.Y < BoardSizeY; P.Y++){
                Func(P);
            }
        }
    }
    catch (...){}
}
/*
template<typename ThingListTy,typename Fn>
inline void IterThroughCombs(ThingListTy & StartList,Fn Fun){
    ThingListTy Temp;
    int size = StartList.size();
    for (int comb_iters : range(2 << size)){
        for (int item : range(size))
            if ((comb_iters >> item) & 1)
                Temp.add((item));
        Fun(Temp);
        Temp.clear();
    }
}*/
