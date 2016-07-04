#pragma once
#include "headerlib/point.hpp"
#include <inttypes.h>

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

constexpr int BoardSizeX = 45;
constexpr int BoardSizeY = 30;
constexpr int XINTERFACE = 60;
constexpr int BUILDINTERFACECOEF = 60;
constexpr int BLOCKSIZE = 30;
constexpr int MONEYHEIGHT = 30;
constexpr int TURNBOXCOEF = 40;
constexpr double HIGHLIGHTSCALEFACTOR = 0.15;
constexpr int MoneyResetVal = 2000;
constexpr int MaximumDomination = 2000000;

//debug preprocessor definitions
//#define DrawRandomPath
//#define DrawMoveValLoc
//#define ShowSquareNums
//#define ShowSubtractEffect
//#define DrawMacroPath

//#define Debug_Macro_Move

#ifdef Debug_Macro_Move
constexpr int DEBUG_MACRO_WIDTH = 30;
constexpr int DEBUG_MACRO_HEIGHT = 20;
#endif

struct Domination{
    int32_t Player;
    int32_t Influence;
    Domination():Player(0),Influence(0){}
    Domination(int32_t inPlayer,int32_t inInfluence):Player(inPlayer),Influence(inInfluence){}
};
using namespace std;
