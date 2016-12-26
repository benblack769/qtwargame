#pragma once
#include <headerlib/Array2d.hpp>
#include "globalinfo.h"

int rep_int(int bsx,int x){
    if(x >= bsx){
        if(x < 2*bsx){
            return x-bsx;
        }
        else{
            return x % bsx;
        }
    }
    else if(x < 0){
        if(x >= -bsx){
            return x + bsx;
        }
        else{
            //guarenteed to be postive, so this terminates on second recursion
            return rep_int((x % bsx) + bsx);
        }
    }
    else{
        return x;
    }
}
int dist_covers_board(int bsx,int dist){
    //if the distance covers the entire board.
    return dist*2+1 >= bsx;
}

Point rep_point(Point bs,Point x){
    //gives back representative point
    return Point(rep_int(bs.X,x.X),rep_int(bs.Y,x.Y));
}

struct RangeData{
    Point start;
    int wrapx;//past the end 
    int stopy;//past the end
    Point bdsize;
    RangeData(Point inboardsize,Point instart,int inwrapx,int instopy):
        start(instart),
        wrapx(inwrapx),
        stopy(instopy),
        bdsize(inboardsize){}
    RangeData(Point inboardsize,Point instart,Point pastend):
        RangeData(inboardsize,instart,pastend.X,pastend.Y){}
    RangeData(Point inboardsize,Point cen,int dist):
        bdsize(inboardsize){
        //if dist is so great that 
        bool x_covers = dist_covers_board(bdsize.X,dist);
        start.X = x_covers  ? 0            : cen.X - dist;
        wrapx  =  x_covers  ? bdsize.X - 1 : cen.X + dist + 1;
        
        bool y_covers = dist_covers_board(bdsize.Y,dist);
        start.Y = y_covers ? 0            : cen.Y - dist;
        stopy  =  y_covers ? bdsize.Y - 1 : cen.Y + dist + 1;
    }
};

class BoardIterator{
protected:
    Point curp;
    RangeData rdata;
public:
    BoardIterator(RangeData inrdata):
        rdata(inrdata),
        curp(inrdata.start){}
    
    void operator *(){
        return curp;
    }
    void operator ++(){
        curp.X++;
        if(curp.X > rdata.bdsize.X){
            curp.X -= rdata.bdsize.X;
        }
        if(curp.X == rdata.wrapx){
            curp.X = rdata.start.X;
            //y mirrors x 
            curp.Y++;
            if(curp.Y > rdata.bdsize.Y){
                curp.Y -= rdata.bdsize.Y;
            }
        }
    }
    bool operator !=(const BoardIterator & )const{
        return stopy != curp.Y;
    }
};
class BoardIterContainer{
protected:
public:
    Board(Point size):
        bsize(size){}
    
};

template<typename ItemTy>
class BoardData:
    FArray2d<ItemTy>{
protected:
    Point bsize;
public:
    BoardData(Point size):
        FArray2d(size.X,size.Y),
        bsize(size){}

    Point bdsize(){
        return bsize;
    }
};

template<typename Ty>
class BoardNums:
        public BoardData
{
public:
    BoardNums(Point size):
        BoardData(size.X,size.Y){}

    //macro hack allows easy making of operator overloads for Array2d
#define _ARRAY2D_OPEQUAL_GENERATOR_(Operator) \
    BoardNums<Ty> & operator Operator (const BoardNums<Ty> & Op2){\
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
    BoardNums<Ty> operator Operator (const BoardNums<Ty> & Op2){\
        BoardNums<Ty> RetVal;\
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
