#pragma once
#include <array>
#include "point.hpp"

template<typename ArrayType,size_t XSize,size_t YSize>
class Array2d
{
public:
	static constexpr size_t ArrSize = XSize*YSize;
        using ArrTy = std::array<ArrayType,ArrSize>;
        ArrTy Arr;
        typedef ArrTy iterator;
	//static version
	Array2d(ArrayType InitVal){
		Assign(InitVal);
	}
        Array2d() = default;
	int size(){
		return Arr.size();
	}
	void assign(ArrayType Val){
            for(ArrayType & v  : Arr){
                v = Val;
            }
	}
	iterator begin(){
		return Arr.begin();
	}
	iterator end(){
		return Arr.end();
	}
	ArrayType & operator [](Point & P){
		return Arr[P.Y*YSize + P.X];
	}
	ArrayType * operator[](int Y){
		return &Arr[Y*YSize];
	}
};
