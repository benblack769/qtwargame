#pragma once
#include "troop.h"
class WallClass:
    public Troop
{
public:
    WallClass(void);
    WallClass(Point InSpot);
    ~WallClass(void){}
    //graphics only extra variable
    bool Vertical;
    void Save(ofstream & File);
    void Load(ifstream & File);
    bool DestroyedAfterAttack(Troop * AttackTroop);
    bool AttemptAttack(Troop * AttackTroop);
    void ResetStats();
};
