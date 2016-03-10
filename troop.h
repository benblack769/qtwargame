#pragma once
#include "globalinfo.h"
#include "troopinfo.h"
enum AttackType{TroopOnly,TroopAndBuild};
class QGraphicsItemGroup;
class Screen;
class Troop
{
protected:
    Point Spot;
    Attach::AttachList Attcs;
public:
    Troop() = default;
    virtual ~Troop();
    int MovementPoints;
    int Hitpoints;
    TInfo::TroopTypes ThisType;
    AttackType TAttackType;
    int AttackValue;
    int Range;
    int ExploRange;
    bool HaveAttacked;
    QGraphicsItemGroup * graphic = nullptr;//Troop owns the pointer
    bool HasAttach(Attach::Attachment Attc){ return Attcs.has(Attc); }
    bool CanHaveAttach(Attach::Attachment Attc){ return TInfo::GetAvaliableAtts(ThisType).has(Attc); }
    virtual void ResetStats() = 0;
    int GetMoveAttackRange(){return MovementPoints + Range;}
    virtual bool DestroyedAfterAttack(Troop * AttackTroop) = 0;
    virtual bool AttemptAttack(Troop * AttackTroop) = 0;
    Point GetSpot(){return Spot;}
    void SetSpot(Point P);
    Attach::AttachList GetAttcs(){return Attcs;}
    virtual void AddAttcs(Attach::Attachment NewA);
    virtual void InitGraphic();

    int GetValue();
    virtual void Save(ofstream & File);
    virtual void Load(ifstream & File);
};
