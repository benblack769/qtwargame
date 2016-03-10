#include "troop.h"
#include <fstream>
#include "interface/screenhelp.h"

Troop::~Troop(){ delete graphic; }
void Troop::SetSpot(Point P){
    if(graphic and this->graphic->pos() != to_pointf(P))
        this->graphic->setPos(to_pointf(P));
    this->Spot = P;
}
void Troop::InitGraphic(){
    if(!this->graphic){
        this->graphic = new QGraphicsItemGroup(fullscreen->getboard());
        this->graphic->setPos(to_pointf(this->Spot));
        this->graphic->setZValue(1);
        //creates a new pixmap with the troop graphic as the parent
        QGraphicsPixmapItem * trooppic = new QGraphicsPixmapItem(trooppics[this->ThisType],this->graphic);
        for(Attach::Attachment A : this->Attcs)
            this->AddAttcs(A);
    }
}
void Troop::AddAttcs(Attach::Attachment NewA){
    if(graphic && !Attcs.has(NewA))
        QGraphicsPixmapItem * attpic = new QGraphicsPixmapItem(attachpics[NewA],this->graphic);
    this->Attcs.add(NewA);
}
int Troop::GetValue(){
    int Cost = TInfo::GetTroopCost(ThisType);
    for (Attach::Attachment Attc : Attcs)
        Cost += Attach::GetAttachCost(Attc);
    return Cost;
}
void Troop::Save(ofstream & File){
    File.write((char *)(&Spot),sizeof(Spot));
    File.write((char *)(&MovementPoints),sizeof(MovementPoints));
    File.write((char *)(&ThisType),sizeof(ThisType));
    File.write((char *)(&Hitpoints),sizeof(Hitpoints));
    File.write((char *)(&TAttackType),sizeof(TAttackType));
    File.write((char *)(&AttackValue),sizeof(AttackValue));
    File.write((char *)(&Range),sizeof(Range));
    File.write((char *)(&ExploRange),sizeof(ExploRange));
    File.write((char *)(&HaveAttacked),sizeof(HaveAttacked));
    File.write((char *)(&Attcs),sizeof(Attcs));
}
void Troop::Load(ifstream & File){
    File.read((char *)(&Spot),sizeof(Spot));
    File.read((char *)(&MovementPoints),sizeof(MovementPoints));
    File.read((char *)(&ThisType),sizeof(ThisType));
    File.read((char *)(&Hitpoints),sizeof(Hitpoints));
    File.read((char *)(&TAttackType),sizeof(TAttackType));
    File.read((char *)(&AttackValue),sizeof(AttackValue));
    File.read((char *)(&Range),sizeof(Range));
    File.read((char *)(&ExploRange),sizeof(ExploRange));
    File.read((char *)(&HaveAttacked),sizeof(HaveAttacked));
    File.read((char *)(&Attcs),sizeof(Attcs));
}
