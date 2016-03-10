#pragma once
#include "pointoperators.h"
template<typename ArrayType>
class RangeArray;

template<typename ArrayType>
class RA_Iterator{
public:
    PointIterator Spot;
    using ArrIterator = typename vector<ArrayType>::iterator;
    ArrIterator ArrIt;
    RA_Iterator(RangeArray<ArrayType> * InArr, bool Begin){
        ArrIt = InArr->Arr.begin();

        int sx = InArr->Corner.X;
        int sy = InArr->Corner.Y;
        int xdis = InArr->XSize;
        int ydis = InArr->YSize;
        Spot = PointIterator(sx, sy, sx + xdis - 1, sy + ydis - 1); //inclusive range here, so -1 is right
        if (!Begin)
            Spot = PointIterator(10, 10, 9, 9);//arbitrary square of negative size
    }
    bool operator != (RA_Iterator & Other){
        return Spot.NotEnd();
    }
    void operator ++ (){
        ++Spot;
        ++ArrIt;
    }
    PointInfo<ArrayType> operator *(){
        return PointInfo<ArrayType>(*Spot, &(*ArrIt));
    }
};
template<typename ArrayType>
class RangeArray
{
public:
    using iterator = RA_Iterator<ArrayType>;
    vector<ArrayType> Arr;
    Point Corner;
    int YSize, XSize;
    RangeArray(Point InCenP,int InRange){
        Corner.X = max(InCenP.X - InRange, 0);
        Corner.Y = max(InCenP.Y - InRange, 0);

        XSize = min(InCenP.X + InRange + 1, BoardSizeX) - Corner.X;
        YSize = min(InCenP.Y + InRange + 1, BoardSizeY) - Corner.Y;

        Arr.resize(XSize * YSize);
        Init(ArrayType());//zeros out data
    }
    RangeArray(BoardSquare Sq) :RangeArray(Sq.Cen, Sq.Range){}
    RangeArray(){
        Corner = CreatePoint(0, 0);
        YSize = 0;
        XSize = 0;
    }
    RangeArray(const RangeArray & other){
        (*this) = other;
    }
    RangeArray(RangeArray && other){
        (*this) = other;
    }

    void operator =(const RangeArray & val){
        _copy_scalars(val);
        Arr = val.Arr;
    }
    void operator =(RangeArray && val){
        _copy_scalars(val);
        Arr = move(val.Arr);
    }
    void Init(ArrayType InitVal){
        for (auto & Val : Arr)
            Val = InitVal;
    }
    int Size(){
        return Arr.size();
    }
    int PointToInt(Point P){
        return (P.X - Corner.X) * YSize + (P.Y - Corner.Y);
    }
    iterator begin(){
        return iterator(this,true);
    }
    iterator end(){
        return iterator(this,false);
    }
    bool IsInScope(Point P){
        int Xadj = P.X - Corner.X;
        int Yadj = P.Y - Corner.Y;
        return Xadj < XSize && Yadj < YSize && Xadj >= 0 && Yadj >= 0;
    }
    ArrayType & operator [](Point & P){
        return Arr[PointToInt(P)];
    }
protected:
    void _copy_scalars(const RangeArray & other){
        Corner = other.Corner;
        YSize = other.YSize;
        XSize = other.XSize;
    }
};
template<typename ArrayType>
class PartialRangeArray;

template<typename ArrayType>
class PointPartialIterator:
    public RA_Iterator<ArrayType>{
public:
    vector<bool>::iterator BIter;
    PointPartialIterator(PartialRangeArray<ArrayType> * InArr, bool Begin) :RA_Iterator<ArrayType>(InArr, Begin){
        BIter = InArr->PointExists.begin();
        ContinueToNextReal();
    }
    void ContinueToNextReal(){
        if (this->Spot.NotEnd() && !(*BIter))
            ++(*this);//coninues on counting if it is not passed the end of itself and it is not at an existing spot
    }
    void operator ++ (){
        RA_Iterator<ArrayType>::operator++();
        ++BIter;
        ContinueToNextReal();
    }
};
template<typename ArrayType>
class PartialIterator{
public:
    vector<bool>::iterator BIter;
    vector<bool>::iterator BEndIt;
    typedef typename vector<ArrayType>::iterator ArrayIterator;
    ArrayIterator ArrIt;
    PartialIterator(PartialRangeArray<ArrayType> * InArr,bool Begin){
        BIter = InArr->PointExists.begin();
        BEndIt = InArr->PointExists.end();
        ArrIt = Begin ? InArr->Arr.begin() : InArr->Arr.end();
        ContinueToNextReal();
    }
    bool operator != (PartialIterator & Other){
        return BIter != BEndIt;
    }
    void ContinueToNextReal(){
        if (BIter != BEndIt && !(*BIter))
            ++(*this);//coninues on counting if it is not passed the end of itself and it is not at an existing spot
    }
    void operator ++ (){
        ++BIter;
        ++ArrIt;
        ContinueToNextReal();
    }
    ArrayType & operator*(){
        return *ArrIt;
    }
};
template<typename ArrayType>
class PartialRangeArray:
    public RangeArray<ArrayType>
{
public:
    using iterator = PointPartialIterator<ArrayType>;
    vector<bool> PointExists;
    using RArray = RangeArray<ArrayType>;
    PartialRangeArray(Point InCenP, int InRange) :RArray(InCenP,InRange){
        int Size = this->XSize * this->YSize;
        PointExists.resize(Size);
        PointExists.assign(Size,0);//assigns every spot to 0, or false
    }
    PartialRangeArray(BoardSquare Sq) :PartialRangeArray(Sq.Cen, Sq.Range){}
    PartialRangeArray() : RArray(){}
    PartialRangeArray(const PartialRangeArray & other){
        (*this) = other;
    }
    PartialRangeArray(PartialRangeArray && other){
        (*this) = other;
    }
    void operator =(const PartialRangeArray & val){
        RArray::operator =(val);
        PointExists = val.PointExists;
    }
    void operator =(PartialRangeArray && val){
        RArray::operator =(val);
        PointExists = move(val.PointExists);
    }
    void SetExist(Point P,bool Val){
        PointExists[this->PointToInt(P)] = Val;
    }
    int ActiveSize(){
        int Count = 0;
        for (bool Boolean : PointExists)
            Count += Boolean;
        return Count;
    }
    bool IsInScope(Point P){
        Point C = this->Corner;
        int Xadj = P.X - C.X;
        int Yadj = P.Y - C.Y;
        if (Xadj < this->XSize && Yadj < this->YSize && Xadj >= 0 && Yadj >= 0)
            return this->PointExists[this->PointToInt(P)];
        else
            return false;
    }
    iterator begin(){
        return iterator(this,true);
    }
    iterator end(){
        return iterator(this, false);
    }

};
