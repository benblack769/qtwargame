#pragma once
#include "globalinfo.h"
//#include <boost/container/flat_set.hpp>
#include <array>
#include <algorithm>
#include <cstring>
#include "helpstuff.h"
template<typename ThingType,const size_t Size>
class ThingVals
{
public:
    array<double, Size> Vals;
    using val_ty = ValInfo<ThingType>;
    using sort_type = array<val_ty,Size>;
    using iterator = typename sort_type::reverse_iterator;
    ThingVals(){
        Vals.fill(-10e10);
        IsSorted = false;
    }
    val_ty GetBest(){
        if(IsSorted)
            return SortedVals.back();
        else{
            InitSortVals();
            return *max_element(SortedVals.begin(),SortedVals.end());
        }
    }
    iterator begin(){
        if(!IsSorted){
            InitSortVals();
            sort(SortedVals.begin(),SortedVals.end());
        }
        return SortedVals.rbegin();
    }
    iterator end(){
        return SortedVals.rend();
    }
    double & operator[](ThingType Attc){
        IsSorted = false;
        return Vals[Attc];
    }
protected:
    bool IsSorted = false;
    sort_type SortedVals;
    void InitSortVals(){
        for (size_t g = 0; g < Size; g++)
            SortedVals[g] = val_ty(Vals[g],static_cast<ThingType>(g));
    }
};
//todo: convert to array
template<typename ThingType,const size_t Size>
class TLIter{
public:
    bool * arr;
    size_t i;
    TLIter(array<bool,Size> & inarr):
        arr(inarr.data()),
        i(0){
        procede();
    }
    bool operator !=(const TLIter &){
        return i < Size;
    }
    void operator ++(){
        i++;
        arr++;
        procede();
    }
    ThingType operator *(){
        return static_cast<ThingType>(i);
    }
    void procede(){
        for(;i < Size && !(*arr);i++,arr++);
    }
};

template<typename ThingType,const size_t Size>
class ThingList{
public:
    using iterator = TLIter<ThingType,Size>;
    array<bool,Size> data;//initializes to all false
    ThingList(){clear();}
    ThingList(initializer_list<ThingType> StartList):ThingList(){
        for (ThingType t : StartList)
            add(t);
    }
    bool has(ThingType TestThing){
        return data[TestThing];
    }
    void add(ThingType TestThing){
        data[TestThing] = true;
    }
    void remove(ThingType TestThing){
        data[TestThing] = false;
    }
    int size(){
        int s = 0;
        for (bool d : data)
            s += d;
        return s;
    }
    void clear(){
        data.fill(false);
    }
    bool operator == (const ThingList & other){
        return memcmp(data.data(),other.data.data(),Size*sizeof(bool)) == 0;
    }
    bool operator !=(const ThingList & other){
        return !(*this == other);
    }
    iterator begin(){
        return iterator(data);
    }
    iterator end(){//this is not nec
        return iterator(data);
    }
};
template<typename ThingTy,const size_t size>
ThingList<ThingTy,size> intersection(ThingList<ThingTy,size> t1,ThingList<ThingTy,size> t2){
    ThingList<ThingTy,size> fin;
    for(int i = 0; i < size; i++)
        fin.data[i] = t1.data[i] & t2.data[i];
    return fin;
}
