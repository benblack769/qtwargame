#pragma once
#include "player.h"
int NumOfTroopDialogue(int max);
Attach::Attachment BuyerDialogue(TInfo::TroopTypes Type,Attach::AttachList Possible_attcs);
bool DemolishDialogue();
class RealPlayer :
    public Player
{
public:
    RealPlayer();
    virtual ~RealPlayer(){}
    SelectTypes SelectedThing;
    Point Selected1;
    Point Selected2;
    int NumOfBuyTroops;
    void RightClick(Point ClickP);
    void LeftClick(Point ClickP);
    void MouseMove(Point MouseLoc);
    bool BuyAttach(int TroopNum);
    void SelectThing(SelectTypes Thing);
    bool ChangeTurn();
    void clear_selection();
};
