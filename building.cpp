#include "building.h"
#include "troop.h"
#include <fstream>
#include "help/helpstuff.h"
#include <RangeIterator.h>
#include "interface/screenhelp.h"
#include <QGraphicsItemGroup>

Building::~Building(){ delete graphic; }
void Building::InitGraphic(){
    if(!this->graphic){
        this->graphic = new QGraphicsItemGroup(fullscreen->getboard());
        this->graphic->setPos(to_pointf(this->DrawCen));

        QGraphicsPixmapItem * buildpic = new QGraphicsPixmapItem(buildpics[this->Type],this->graphic);
    }
}
int GetBuildingCost(BuildingTypes Build){
    switch (Build){
    case SFarm:return 50;
    case LFarm:return 100;
    case CataFact:return 500;
    case TrainCenter:return 100;
    case BA_Factory:return 200;
    case TorchFactory:return 200;
    case Armory:return 300;
    case Stable:return 500;
    }
    return 1000000;//should not happen
}
int GetBuildingSize(BuildingTypes Build){
    switch (Build){
    case SFarm:return 4;
    case LFarm:return 4;
    case CataFact:return 4;
    case TrainCenter:return 4;
    case BA_Factory:return 4;
    case TorchFactory:return 4;
    case Armory:return 4;
    case Stable:return 4;
    }
}
int GetBuildingIncome(BuildingTypes Build){
    if(Build == SFarm)
        return 3;
    else if(Build == LFarm)
        return 5;
    else
        return 0;
}
int BuyAmmountConverter(BuildingTypes Build){
    switch (Build){
    case SFarm:return 0;
    case LFarm:return 0;
    case CataFact:return 1;
    case TrainCenter:return 5;
    case BA_Factory:return 5;
    case TorchFactory:return 5;
    case Armory:return 5;
    case Stable:return 5;
    }
}
Building::Building(BuildingTypes InBuildType,int x1,int y1,int x2, int y2){
    Type = InBuildType;
    Point low{min(x1,x2),min(y1,y2)};
    Point high{max(x1,x2),max(y1,y2)};
    int BuildSize = GetBuildingSize(Type);

    if(BuildSize == 4)
        Place = vector<Point>({low,Point{high.X,low.Y},Point{low.X,high.Y},high});

    DrawCen = Place[0];
    BuyAmmount = 0;//makes it so that the player has to wait a turn to use this building
}
void Building::Save(ofstream & File){
    File.write((char *)(&Type),sizeof(Type));
    File.write((char *)(&BuyAmmount),sizeof(BuyAmmount));
    int NumOfPlaces = Place.size();
    File.write((char *)(&NumOfPlaces), sizeof(NumOfPlaces));
    File.write((char *)(&DrawCen), sizeof(DrawCen));
    File.write((char *)(Place.data()), sizeof(Point)*Place.size());
}
void Building::Load(ifstream & File){
    File.read((char *)(&Type),sizeof(Type));
    File.read((char *)(&BuyAmmount),sizeof(BuyAmmount));
    int NumOfPlaces;
    File.read((char *)(&NumOfPlaces), sizeof(NumOfPlaces));;
    File.read((char *)(&DrawCen), sizeof(DrawCen));
    Place.resize(NumOfPlaces);
    File.read((char *)(Place.data()), sizeof(Point)*Place.size());
}
void Building::ResetStats(){
    BuyAmmount = BuyAmmountConverter(Type);
}
int Building::GetCoordVar(Point Spot){
    for(int CVar :range(Place.size())){
        if (Place[CVar] == Spot)
            return CVar;
    }
}
int Building::GetNumOfSquares(){
    return Place.size();
}
bool Building::GetOverallExist(){
    return Place.size() == 0;
}
bool Building::DestroyedAfterAttack(Point Loc,Troop * AttackingTroop){
    if(AttackingTroop->TAttackType != TroopAndBuild)
        return false;

    int Coord = GetCoordVar(Loc);
    Place.erase(Place.begin() + Coord);

    return (Place.size() == 0);
}
bool Building::AttemptAttack(Troop * AttackingTroop){
    return (AttackingTroop->TAttackType == TroopAndBuild);
}
void Building::IterOverBuildFromDis(Array2d<int> & PlayerOcc,int DistanceFromBuild, function<void(Point)> IterFunc){
    Point Cen = GetAvPoint();
    //gets the maximum distance from the average point(not necessarily an actual part of the building!)
    int MaxDis = 0;
    for (Point BP : Place)
        MaxDis = max(MaxDis, GetBoardDistance(Cen, BP));

    int TDis = MaxDis + DistanceFromBuild;
    for (Point PosPoint : SquareIterate(Cen,TDis)){
        if (!BlankPoint(PosPoint))
            continue;
        //makes the final check that this square is valid
        for (Point BP : Place){
            if (GetBoardDistance(BP,PosPoint) <= DistanceFromBuild){
                IterFunc(PosPoint);
                break;
            }
        }
    }
}
Point Building::GetAvPoint(){
    Point CenP = {0,0};
    for(Point & P : Place){
        CenP += P;
    }
    CenP /= Place.size();
    return CenP;
}
int Building::GetIncome(){
    return GetBuildingIncome(Type);
}

