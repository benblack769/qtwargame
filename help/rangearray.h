#pragma once
#include "pointoperators.h"
#include <headerlib/range_array.hpp>
template<typename ArrayType>
class PartialRangeArray;

template<typename ArrayType>
class PointPartialIterator:
    public RA_Iterator<ArrayType>{
public:
    vector<bool>::iterator BIter;
    PointPartialIterator(PartialRangeArray<ArrayType> * InArr) :
        RA_Iterator<ArrayType>(InArr){
        BIter = InArr->PointExists.begin();
        ContinueToNextReal();
    }
    PointPartialIterator(): RA_Iterator<ArrayType>(){}
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
    PartialRangeArray(ConstSquare Sq) :PartialRangeArray(Sq.Cen, Sq.Range){}
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
        return iterator(this);
    }
    iterator end(){
        return iterator();
    }

};
