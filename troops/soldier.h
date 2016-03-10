#pragma once
#include "troop.h"
class Soldier:
    public Troop
{
public:
    Soldier();
    Soldier(Point InSpot);
    ~Soldier(void) = default;
    bool DestroyedAfterAttack(Troop * AttackTroop);
    bool AttemptAttack(Troop * AttackTroop);
    void ResetStats();
};

