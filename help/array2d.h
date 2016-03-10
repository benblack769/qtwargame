#pragma once
template<typename ArrayType>
class Array2d
{
public:
    //static version
    Array2d(ArrayType InitVal){
        Init(InitVal);
    }
    Array2d(void){
        //zeros out memory
        fill(begin(), end(), ArrayType());
    }
    ArrayType Arr[BoardSizeX][BoardSizeY];
    int Size(){
        return sizeof(ArrayType) * BoardSizeX * BoardSizeY;
    }
    void Init(ArrayType InitVal){
        fill(begin(), end(), InitVal);
    }
    ArrayType * begin(){
        return Arr[0];
    }
    ArrayType * end(){
        return &Arr[BoardSizeX - 1][BoardSizeY];
    }
    void operator = (const Array2d & InArr){
        //copy_n(InArr.begin(), BoardSizeX * BoardSizeY,begin());
        memcpy(Arr, InArr.Arr, BoardSizeX * BoardSizeY * sizeof(ArrayType));
    }
    ArrayType & operator [](Point P){
        return Arr[P.X][P.Y];
    }
    ArrayType * operator[](int X){
        return Arr[X];
    }
    typedef ArrayType Ty;
    //macro hack allows easy making of operator overloads for Array2d
#define _ARRAY2D_OPEQUAL_GENERATOR_(Operator) \
    void operator Operator (const Array2d<Ty> & Op2){\
        for(int x = 0; x < BoardSizeX; x++)\
            for (int y = 0; y < BoardSizeY; y++)\
                Arr[x][y] Operator Op2.Arr[x][y];\
        }
    _ARRAY2D_OPEQUAL_GENERATOR_(+= )
    _ARRAY2D_OPEQUAL_GENERATOR_(*= )
    _ARRAY2D_OPEQUAL_GENERATOR_(/= )
    _ARRAY2D_OPEQUAL_GENERATOR_(-= )
#undef _ARRAY2D_OPEQUAL_GENERATOR_
#define _ARRAY2D_OP_GENERATOR_(Operator) \
    Array2d<Ty> operator Operator (const Array2d<Ty> & Op2){\
        Array2d<Ty> RetVal(0);\
        for(int x = 0; x < BoardSizeX; x++)\
            for (int y = 0; y < BoardSizeY; y++)\
                RetVal.Arr[x][y] = Arr[x][y] Operator Op2.Arr[x][y];\
        }
    _ARRAY2D_OP_GENERATOR_(+)
    _ARRAY2D_OP_GENERATOR_(*)
    _ARRAY2D_OP_GENERATOR_(/ )
    _ARRAY2D_OP_GENERATOR_(-)
#undef _ARRAY2D_OP_GENERATOR_
};
