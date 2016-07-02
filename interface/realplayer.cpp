#include "realplayer.h"
#include "help/pointoperators.h"
#include <algorithm>
#include <headerlib/RangeIterator.h>
#include <QMessageBox>
#include <QInputDialog>
#include "interface/screenhelp.h"

RealPlayer::RealPlayer(void):Player(){
    Type = Real;
    SelectedThing = NoSelect;
    Selected1 = { -1, -1 };
    Selected2 = { -1, -1 };
}
bool RealPlayer::BuyAttach(int TroopNum){
    Troop * T = Troops[TroopNum];
    Attach::AttachList possibles;
    for(Attach::Attachment a : Attach::FullAttachList)
        if (GetBuyAmmounts(AttBuildReqs(a),T->GetSpot()) > 0)
            possibles.add(a);
    Attach::Attachment Attc = BuyerDialogue(T->ThisType,possibles);
    if (Attc == Attach::NoAttach)
        return false;
    return Player::BuyAttach(Attc,T);
}
void RealPlayer::MouseMove(Point MouseLoc){
    if(MiniTurn == 0 and Selected1.X >= 0 and SelectedThing == NoSelect){
        fullscreen->emph->clear_sel();
        vector<Point> spots = GetValidDistances(Buildings[NPlayerMap[Selected1]],MouseLoc);
        if(NumOfBuyTroops < spots.size())
            spots.resize(NumOfBuyTroops);
        fullscreen->emph->add_squares(spots,Qt::blue);
    }
}
void RealPlayer::SelectThing(SelectTypes Thing){
    // makes the player "pick up" a potencial building
    // player does not pay for building, but it does chech if they have enough
    // match picture with selected building
    clear_selection();
    if (MiniTurn != 0)
        return;
    BuildingTypes Build = SelectToBuild(Thing);
    TInfo::TroopTypes TType = SelectToTroop(Thing);
    int Cost = 0;
    if (Build != NoBuild){
        Cost = GetBuildingCost(Build);
    }
    else if (TType != TInfo::NoType){
        Cost = TInfo::GetTroopCost(TType);
    }
    if (Cost > Money)
        MakeDialogue("Sorry, but you do not have enough money to purchace this", "Not Enough Money");
    else{
        fullscreen->emph->add_build_choice(Thing,Qt::blue);
        SelectedThing = Thing;
    }
}
void RealPlayer::RightClick(Point ClickP){
    if(MiniTurn == 0){//Building Part of turn
        if(TPlayerMap[ClickP] == BuildingType){
            fullscreen->emph->add_squares(Buildings[NPlayerMap[ClickP]]->Place,Qt::gray);
            if(DemolishDialogue()){
                RemoveThingCompletely(BuildingType,NPlayerMap[ClickP]);
            }
            fullscreen->emph->clear_sel();
        }
        if(BlankPoint(ClickP) && SelectedThing != NoSelect){
            if(Selected1.X < 0){
                fullscreen->emph->add_square(ClickP,Qt::blue);
                Selected1 = ClickP;
            }
            else if(Selected2.Y < 0){
                if (SelectToBuild(SelectedThing) != NoBuild)
                    PlaceNewBuilding(SelectToBuild(SelectedThing), Selected1.X, Selected1.Y, ClickP.X,ClickP.Y);
                else if (SelectToTroop(SelectedThing) == TInfo::WallType)
                    PlaceWall(Selected1, ClickP);
                SelectedThing = NoSelect;

                Selected1 = ClearPoint();
                fullscreen->emph->clear_sel();
                ShowOnScreen();
            }
        }
    }
    else if(MiniTurn == 1){//moving part of turn

    }
    else if(MiniTurn == 2){//attacking part of turn

    }
}
void RealPlayer::LeftClick(Point ClickP) {
    if(MiniTurn == 0){//Building Part of turn
        //acts as a way to clear the selection by clicking somewhere else
        if(SelectedThing != NoSelect){
            clear_selection();
            return;
        }
        //allows the player to click to center the troop dispach, and finalize the purchase
        //is all about buildings, nothing else should be able to be selected in Miniturn 0
        //selectedsquare1 is the place where the person selectes the building that will produce troops
        if(Selected1.X >= 0){
            int BuildNum = NPlayerMap[Selected1];
            Building * B = Buildings[BuildNum];
            BuildingTypes Build = B->Type;
            vector<Point> spots = GetValidDistances(B,ClickP);
            if (spots.size() < NumOfBuyTroops)
                MakeDialogue("Not enough room for all troops.","Error");

            int num_bought = min(spots.size(),NumOfBuyTroops);

            B->BuyAmmount -= num_bought;
            for(int i : range(num_bought)){
                PlaceNewTroop(spots[i], GetTroopType(Build));
            }
            clear_selection();
            ShowOnScreen();
        }
        int Num = NPlayerMap[ClickP];
        BaseType ThingType = TPlayerMap[ClickP];
        if(ThingType == BuildingType && TrainBuilds.has(Buildings[Num]->Type)){
            //controls the selection of the building of troops
            Building * B = Buildings[Num];

            TInfo::TroopTypes TroopType = GetTroopType(B->Type);
            NumOfBuyTroops = NumOfTroopDialogue(B->BuyAmmount);
            if (NumOfBuyTroops * GetTroopCost(TroopType) > Money){
                MakeDialogue("Not enough money for all troops.","Error");
                NumOfBuyTroops = Money / GetTroopCost(TroopType);
            }
            if(NumOfBuyTroops != 0)
                Selected1 = ClickP;
        }
        else if(ThingType == TroopType){
            BuyAttach(Num);
        }
    }
    else if(MiniTurn == 1){//moving part of turn
        if(Selected1.X >= 0){
            if(BlankPoint(ClickP) and TPlayerMap[Selected1] == TroopType){
                MoveTroop(NPlayerMap[Selected1],ClickP);
            }
            clear_selection();
        }
        int Num = NPlayerMap[ClickP];
        BaseType ThingType = TPlayerMap[ClickP];
        if(ThingType == TroopType){
            if(Troops[Num]->MovementPoints == 0){
                MakeDialogue("This unit has no movement points left","Error");
            }
            else{
                Selected1 = ClickP;
            }
        }
    }
    else if(MiniTurn == 2){//attacking part of turn
        if(Selected1.X >= 0){
            int TroopNum = NPlayerMap[Selected1];
            BaseType TType = TPlayerMap[Selected1];
            if(TType == TroopType){
                if(Troops[TroopNum]->ExploRange == 1){
                    if(AttemptAttackOnOthers(TroopNum,ClickP))
                        AttackLocation(ClickP,Troops[TroopNum]);
                }
                else{
                    Selected2 = ClickP;
                    return;
                }
            }
        }
        if(Selected2.X >= 0){
            //Todo: implement multiple explosion squares
        }
        int Num = NPlayerMap[ClickP];
        BaseType ThingType = TPlayerMap[ClickP];
        if(ThingType == TroopType){
            if(Troops[Num]->HaveAttacked == true){
                MakeDialogue("This unit has already attacked this turn","Error");
            }
            else{
                Selected1 = ClickP;
            }
        }
    }
}
void RealPlayer::clear_selection(){
    Selected1 = ClearPoint();
    Selected2 = ClearPoint();
    SelectedThing = NoSelect;
    fullscreen->emph->clear_sel();
}
bool RealPlayer::ChangeTurn(){
    clear_selection();
    return Player::ChangeTurn();
}

int NumOfTroopDialogue(int max){
    return QInputDialog::getInt(nullptr,
                                "Troop Purchase",
                                "How many troops do you want to buy?",
                                0, //def value
                                0, //min value
                                max);// max value
}
Attach::Attachment BuyerDialogue(TInfo::TroopTypes Type,Attach::AttachList Possible_attcs){
    // creates a list of strings of the avaliable attachments
    QStringList sl;
    for(Attach::Attachment attc : intersection(TInfo::GetAvaliableAtts(Type),Possible_attcs))
        sl.push_back(Attach::attc_strs[attc].c_str());
    bool clicked_ok = true;
    //runs the dialog
    QString s = QInputDialog::getItem(nullptr,
                              "Attachment Purchase",
                              "Which attachment would you like to buy",
                              sl,0,false,&clicked_ok);
    if(!clicked_ok)
        return Attach::NoAttach;

    //checks which attachment the string is associated with by liniar search (maybe not best)
    string sout = s.toStdString();
    for (int i : range(NumOfAttachments))
        if (sout == Attach::attc_strs[i])
            return static_cast<Attach::Attachment>(i);

    return Attach::NoAttach;//shouldn't happen
}
bool DemolishDialogue(){
    QMessageBox b(QMessageBox::Question,
                    "Demolish",
                    "Are you sure you want to demolish this building?",
                    QMessageBox::Yes | QMessageBox::No);
    return b.exec() == QMessageBox::Yes;
}
