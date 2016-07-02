#include "screen.h"
#include <QGraphicsItemGroup>
#include <QPixmap>
#include <QString>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QInputDialog>

#include <headerlib/RangeIterator.h>
#include "help/pointoperators.h"
#include "help/helpstuff.h"

#include "troopinfo.h"
#include "player.h"
#include "base.h"
#include "screenhelp.h"

Screen * fullscreen;

QColor clear_c(0,0,0,0);

QPixmap * backpic;
QPixmap * highlightpic;

QList<QPixmap> selecticons;
QList<QPixmap> buildpics;
QList<QPixmap> trooppics;
QList<QPixmap> attachpics;


void load_imgs(){
    backpic = new QPixmap(":/Pictures/Background.bmp");
    highlightpic = new QPixmap(":/Pictures/Highlight.png");

    selecticons = QList<QPixmap>({
    QPixmap(":/Pictures/SmallFarm.bmp"),
    QPixmap(":/Pictures/LargeFarm.bmp"),
    QPixmap(":/Pictures/CatFactory.bmp"),
    QPixmap(":/Pictures/TrainCenter.bmp"),
    QPixmap(":/Pictures/BA_Factory.bmp"),
    QPixmap(":/Pictures/TorchFactory.bmp"),
    QPixmap(":/Pictures/Armory.bmp"),
    QPixmap(":/Pictures/Stable.bmp"),
    QPixmap(":/Pictures/WallIcon.bmp")});

    buildpics = selecticons;

    trooppics = QList<QPixmap>({
    QPixmap(":/Pictures/Soldier.png"),
    QPixmap(":/Pictures/Catapult.png"),
    QPixmap(":/Pictures/WallMiddle.bmp")});

    attachpics = QList<QPixmap>({
    QPixmap(),//stands in for noattach. Shouldn't really be drawn
    QPixmap(":/Pictures/Torch.png"),
    QPixmap(":/Pictures/BA.png"),
    QPixmap(":/Pictures/Horse.png"),
    QPixmap(":/Pictures/Armor.png")});
}

Screen::Screen(){
    fullscreen = this;
    this->setBackgroundBrush(QBrush(background));

    board = new BoardG;
    this->addItem(board);

    miniturn = new MiniturnG;
    this->addItem(miniturn);

    buildchoices = new BuildChooseG;
    this->addItem(buildchoices);

    moneybox = new MoneyG;
    this->addItem(moneybox);

    emph = new SelectEmphasis;
    this->addItem(emph);

    board->setPos(XINTERFACE,0);
    miniturn->setPos(0,100);
    miniturn->setZValue(1);
    moneybox->setPos(0,50);
}
void Screen::placepic(QGraphicsItemGroup * group,const QPixmap & pic,int xpix,int ypix){
    QGraphicsPixmapItem * block = this->addPixmap(pic);
    block->setOffset(xpix,ypix);
    group->addToGroup(block);
}
void Screen::resize(QRectF rect){
    this->miniturn->setPos(0,rect.bottom()-TURNBOXCOEF*3);
    this->moneybox->setPos(0,rect.bottom()-TURNBOXCOEF*3-MONEYHEIGHT);
}
void Screen::keyPressEvent(QKeyEvent * event){
    int k = event->key();
    switch(k){
    case Qt::Key_Plus:
        this->buildchoices->changeBuildPos(1);
        break;
    case Qt::Key_Minus:
        this->buildchoices->changeBuildPos(-1);
        break;
    case Qt::Key_Up:
        this->board->move_board(Point{0,-1});
        break;
    case Qt::Key_Down:
        this->board->move_board(Point{0,1});
        break;
    case Qt::Key_Right:
        this->board->move_board(Point{1,0});
        break;
    case Qt::Key_Left:
        this->board->move_board(Point{-1,0});
        break;
    }
}
void Screen::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    //this function currently does nothing at all
    QGraphicsScene::mousePressEvent(mouseEvent);
}
void Screen::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    GetCurrentPlayer()->MouseMove(board->screen_to_point(mouseEvent->scenePos()));
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

