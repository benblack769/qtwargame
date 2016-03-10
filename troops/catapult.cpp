#include "catapult.h"

Catapult::Catapult(Point InSpot):Catapult(){
    SetSpot(InSpot);
}
Catapult::Catapult(){
    ThisType = TInfo::CatapultType;

    //when you create a catapult, it cannot move or attack, but it does defend itself
    ResetStats();
    MovementPoints = 0;
    HaveAttacked = true;
}
void Catapult::ResetStats(){
    MovementPoints = 2 + Attcs.has(Attach::CataHorse)*2;
    Hitpoints = 1;
    TAttackType = TroopAndBuild;
    AttackValue = 1;
    ExploRange = 1;
    Range = 5;
    HaveAttacked = false;
}
bool Catapult::DestroyedAfterAttack(Troop * AttackTroop){
    Hitpoints -= AttackTroop->AttackValue;
    if(Hitpoints <= 0)
        return true;
    else
        return false;
}
bool Catapult::AttemptAttack(Troop * AttackTroop){
    return true;
}

