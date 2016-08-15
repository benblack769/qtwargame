#pragma once
#include <headerlib/Array2d.hpp>
#include "globalinfo.h"
template<typename Ty>
class BoardArray:
        public Array2d<Ty,BoardSizeX,BoardSizeY>
{
public:
    BoardArray(Ty Initval):
        Array2d<Ty,BoardSizeX,BoardSizeY>(Initval)
    {}
    BoardArray():
        BoardArray(Ty())
    {}
    //macro hack allows easy making of operator overloads for Array2d
#define _ARRAY2D_OPEQUAL_GENERATOR_(Operator) \
    BoardArray<Ty> & operator Operator (const BoardArray<Ty> & Op2){\
        for(size_t i = 0; i < this->size(); i++){\
            this->Arr[i] Operator Op2.Arr[i];\
        }\
        return *this;\
    }
    _ARRAY2D_OPEQUAL_GENERATOR_(+= )
    _ARRAY2D_OPEQUAL_GENERATOR_(*= )
    _ARRAY2D_OPEQUAL_GENERATOR_(/= )
    _ARRAY2D_OPEQUAL_GENERATOR_(-= )
#undef _ARRAY2D_OPEQUAL_GENERATOR_
#define _ARRAY2D_OP_GENERATOR_(Operator) \
    BoardArray<Ty> operator Operator (const BoardArray<Ty> & Op2){\
        BoardArray<Ty> RetVal;\
        for(size_t i = 0; i < this->size(); i++){\
            RetVal.Arr[i] = this->Arr[i] Operator Op2.Arr[i];\
        }\
        return RetVal;\
    }
    _ARRAY2D_OP_GENERATOR_(+)
    _ARRAY2D_OP_GENERATOR_(*)
    _ARRAY2D_OP_GENERATOR_(/ )
    _ARRAY2D_OP_GENERATOR_(-)
#undef _ARRAY2D_OP_GENERATOR_
};
