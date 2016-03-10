#pragma once

constexpr int MaxMoveDis = 12;
#include "globalinfo.h"
#include "help/thingstuff.h"

namespace Attach{
    //make sure that these values go from 0 to NumOfAttachments
    enum Attachment{ NoAttach = 0, Torch = 1, BA = 2, Horse = 3, Armor = 4, CataHorse = 5};//change GlobVars!
    const string attc_strs[] = {"", "Torch", "BA", "Horse", "Armor", "CataHorse"};
#define NumOfAttachments 6
    constexpr int AttachBuyRange = 5;
    typedef ThingList<Attachment,NumOfAttachments> AttachList;
    extern AttachList FullAttachList;
    typedef ThingVals<Attachment, NumOfAttachments> AttachVals;
    inline int GetAttachCost(Attachment Attc){
        switch (Attc){
        case Horse:return 50;
        case Torch:return 20;
        case BA:return 20;
        case Armor:return 30;
        case CataHorse:return 100;
        default:return 0;
        }
    }
}
namespace TInfo{
    //make sure that these values go from 0 to NumOfAttachments
    enum TroopTypes{NoType = -1,SoldierType = 0,CatapultType = 1,WallType = 2};//change GlobVars!
    constexpr int NumOfTroopsTypes = 3;
    typedef ThingVals<TroopTypes, NumOfTroopsTypes> TroopVals;//this will be used by the computer player group system
    typedef ThingList<TroopTypes,NumOfTroopsTypes> TroopList;
    extern TroopList FullTroopList;//change this when changing TroopTypes
    inline int GetTroopCost(TroopTypes TType){
        switch (TType){
        case SoldierType:return 15;
        case CatapultType:return 100;
        case WallType:return 75;
        default:return 0;
        }
    }
    inline Attach::AttachVals GetDefaultAttachVals(TroopTypes TType){
        using namespace Attach;
        Attach::AttachVals TroopVals;
        int TroopVal = GetTroopCost(TType);
        switch (TType){
        case SoldierType:
            TroopVals[BA] = TroopVal;//says that it doubles effectiveness of troop
            TroopVals[Armor] = TroopVal / 2;
            TroopVals[Horse] = TroopVal / 4;
            TroopVals[Torch] = TroopVal / 8;
            break;
        case CatapultType:
            TroopVals[CataHorse] = TroopVal/3;//says that it increases effectiveness of troop by 33%
            break;
        default:
            break;
        }
        return TroopVals;
    }
    inline Attach::AttachList GetAvaliableAtts(TroopTypes TType){
        using namespace Attach;
        switch (TType){
        case SoldierType:
            return{ Torch, BA, Horse, Armor };
        case CatapultType:
            return{ Horse };
        default:
            return{};
        };
    }
}
//namespace BInfo{
    constexpr int NumOfBuildingTypes = 8;
    //change GlobVars! when you change this
    enum BuildingTypes {NoBuild = -1,SFarm = 0, LFarm = 1, CataFact = 2, TrainCenter = 3, BA_Factory = 4, TorchFactory = 5, Armory = 6, Stable = 7};
    typedef ThingVals<BuildingTypes, NumOfBuildingTypes> BuildVals;
    typedef ThingList<BuildingTypes,NumOfBuildingTypes> BuildList;
    extern BuildList FullBuildList;
    extern BuildList AttachBuilds;
    extern BuildList IncomeBuilds;
    extern BuildList TrainBuilds;
//};
constexpr int NumOfSelectTypes = 9;
enum SelectTypes{NoSelect = -1,SFarmSelect = 0, LFarmSelect = 1, CataFactSelect = 2, TrainCenterSelect = 3, BA_FactorySelect = 4, TorchFactorySelect = 5, ArmorySelect = 6, StableSelect = 7, WallSelect = 8};

inline bool IsBuild(SelectTypes Type){
    return Type < NumOfBuildingTypes;
}
inline BuildingTypes SelectToBuild(SelectTypes Type){
    if (IsBuild(Type))
        return static_cast<BuildingTypes>(Type);
    else
        return NoBuild;
}
inline TInfo::TroopTypes SelectToTroop(SelectTypes Type){
    switch (Type){
    case WallSelect:return TInfo::WallType;
    default:return TInfo::NoType;
    }
}
