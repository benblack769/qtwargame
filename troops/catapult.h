#pragma once
#include "troop.h"
class Catapult:
    public Troop
{
public:
    Catapult(void);
    Catapult(Point InSpot);
    ~Catapult(void) = default;
    void ResetStats();
    bool DestroyedAfterAttack(Troop * AttackTroop);
    bool AttemptAttack(Troop * AttackTroop);
};
