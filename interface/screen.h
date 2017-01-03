#pragma once
#include <QGraphicsScene>
//#include <QPixmap>
class QPixmap;
#include "globalinfo.h"

class QGraphicsItemGroup;
class BoardG;
class BuildChooseG;
class MiniturnG;
class MoneyG;
class SelectEmphasis;
class PlayerDomDisplay;

void load_imgs();
class Screen : public QGraphicsScene
{
public:
    BoardG *board;
    BuildChooseG *buildchoices;
    MiniturnG *miniturn;
    MoneyG *moneybox;
    SelectEmphasis * emph;
    PlayerDomDisplay * pdomdis;

    QColor background = Qt::black;
    Screen();
    void placepic(QGraphicsItemGroup * group,const QPixmap & pic,int xpix,int ypix);
    void resize(QRectF rect);
    void draw_playerdom();
    BoardG * getboard(){return board;}
protected:
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
};
