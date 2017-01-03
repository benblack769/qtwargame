#include "screenhelp.h"
#include "help/helpstuff.h"
#include <QGraphicsSceneMouseEvent>
#include "base.h"

BoardG::BoardG(QGraphicsItem * parent):QGraphicsItemGroup(parent),offp({0,0}){
    create_background();
}
void BoardG::placepic(const QPixmap & pic,Point P){
    fullscreen->placepic(this,pic,P.X*BLOCKSIZE,P.Y*BLOCKSIZE);
}
void BoardG::create_background(){
    for(Point P : BoardIterate())
        this->placepic(*backpic,P);
}
void BoardG::move_board(Point adj){
    Point n = offp + adj;
    if(OnBoard(n + Point{5,5})){
        offp = n;
        this->setPos(this->pos() - to_pointf(adj));
    }
}
void BoardG::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    QPointF actp = mouseEvent->pos();// - this->pos();

    Point BP = board_to_point(actp);
    Player * curp = GetCurrentPlayer();
    if(mouseEvent->button() == Qt::RightButton)
        curp->RightClick(BP);
    else if(mouseEvent->button() == Qt::LeftButton)
        curp->LeftClick(BP);
}
QColor player_to_color(int pnum){
    switch(pnum){
    case 0:return QColor::blue();
    case 1:return QColor::red();
    case 2:return QColor::yellow();
    case 3:return QColor::green();
    }
}

PlayerDomDisplay::PlayerDomDisplay(BoardG * boardg){
    for(Point P : BoardIterate()){
        Domination pdom = PlayerDom[P];
        qreal opacity = pdom.Influence/qreal(MaximumDomination);
        DrawColorSquare(player_to_color(pdom.Player),P,opacity);
    }
} 

void PlayerDomDisplay::DrawColorSquare(QColor c,Point P,qreal opacity){
    QGraphicsRectItem * ptr = new QGraphicsRectItem(to_squaref(P),this);
    ptr->setBrush(QBrush(c));
    ptr->setOpacity(opacity);
    ptr->setZValue(50);
}

MoneyG::MoneyG(QGraphicsItem * parent):QGraphicsItemGroup(parent){
    this->addToGroup(fullscreen->addRect(0,0,XINTERFACE,MONEYHEIGHT,QPen(Qt::black),QBrush(Qt::white)));
    moneytext = fullscreen->addText(QString(),QFont("Bodoni MT"));
    this->addToGroup(moneytext);
}
void MoneyG::setmoney(int value){
    moneytext->setPlainText(QString("$")+QString::number(value));
}

MiniturnG::MiniturnG(QGraphicsItem * parent):QGraphicsItemGroup(parent){
    this->addToGroup(fullscreen->addRect(0,0,XINTERFACE,3*TURNBOXCOEF,QPen(),QBrush(Qt::white)));
    makebox(QString("Build"),QFont("Arial Rounded MT Bold"),Qt::blue,0);
    makebox(QString("Move"),QFont("Bauhaus 93"),Qt::green,1);
    makebox(QString("Attack"),QFont("Algerian"),Qt::red,2);
    miniturn = 0;
}
void MiniturnG::makebox(const QString & str,const QFont & f,QColor color,int ypos){
    int ypix = ypos*TURNBOXCOEF;
    QGraphicsRectItem * back = fullscreen->addRect(0,ypix,XINTERFACE,TURNBOXCOEF,QPen(),QBrush(color));
    QGraphicsTextItem * chosebox = fullscreen->addText(str,f);
    chosebox->setPos(0,ypix);
    back->setOpacity(fadeop);
    backs[ypos] = back;
    this->addToGroup(back);
    this->addToGroup(chosebox);
}
void MiniturnG::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    int oldmt = miniturn;
    miniturn = (miniturn + 1) % 3;
    backs[oldmt]->setOpacity(fadeop);
    backs[miniturn]->setOpacity(1.0);
    ChangeMiniTurn();
}
BuildChooseG::BuildChooseG(QGraphicsItem * parent):QGraphicsItemGroup(parent){
    for(int yloc : range(NumOfSelectTypes))
        fullscreen->placepic(this,selecticons[yloc],0,yloc*BUILDINTERFACECOEF);
}
void BuildChooseG::changeBuildPos(int amount){
    int npos = buildpos+amount;
    if(npos >= 0 && npos < NumOfSelectTypes){
        this->setY(this->y() + -amount*BUILDINTERFACECOEF);
        buildpos = npos;
    }
}
void BuildChooseG::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    int click_loc = (mouseEvent->pos() / BUILDINTERFACECOEF).y();
    int build_loc = click_loc;
    if (build_loc >= NumOfSelectTypes or build_loc < 0)
        return;
    SelectTypes selb = static_cast<SelectTypes>(build_loc);
    GetCurrentPlayer()->SelectThing(selb);
}

class high_rect:
        public QGraphicsItemGroup{
public:
    high_rect(QGraphicsItem * parent=nullptr):
        QGraphicsItemGroup(parent){}
    high_rect(QGraphicsItem * parent,QColor color,QPointF pos,QPointF size):high_rect(parent){
        init(color,pos,size);
    }
    //makes a gradient emphasized square
    void init(QColor color,QPointF pos,QPointF size){
        QPointF scaled_size = size * HIGHLIGHTSCALEFACTOR;
        for(Point dir : {Point{0,1},Point{0,-1},Point{1,0},Point{-1,0}}){
            QPointF Start = dir.X < 0 or dir.Y < 0 ?
                        size :
                        QPointF(0,0);
            QPointF End = Start + QPointF(scaled_size.x() * dir.X,scaled_size.y()*dir.Y);
            QLinearGradient x(Start,End);
            x.setColorAt(0,color);
            x.setColorAt(1,clear_c);

            QBrush b(x);

            QGraphicsRectItem * rect = fullscreen->addRect(QRectF(QPointF(0,0),size),QPen(clear_c),b);
            rect->setParentItem(this);
            rect->setPos(pos);
        }
    }
};

SelectEmphasis::SelectEmphasis(QGraphicsItem * parent):QGraphicsItemGroup(parent){}
void SelectEmphasis::add_rect(QGraphicsItem * parent,QPointF up_l,QPointF size, QColor color){
    this->sel_items.append(new high_rect(parent,color,up_l,size));
}
void SelectEmphasis::clear_sel(){
    for(QGraphicsItem * w : sel_items)
        delete w;
    sel_items.clear();
}
void SelectEmphasis::add_rect(Point up_left,Point bot_right,QColor color){
    add_rect(fullscreen->board,to_pointf(up_left),to_pointf(bot_right - up_left),color);
}
void SelectEmphasis::add_square(Point p,QColor color){
    add_rect(p,p + Point{1,1},color);
}
void SelectEmphasis::add_squares(vector<Point> squares,QColor color){
    for(Point P : squares)
        add_square(P,color);
}
void SelectEmphasis::add_build_choice(SelectTypes choice,QColor color){
    add_rect(fullscreen->buildchoices,
             QPointF(0,choice*BUILDINTERFACECOEF),
             QPointF(XINTERFACE,BUILDINTERFACECOEF),color);
}
