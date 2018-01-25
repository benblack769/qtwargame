#pragma once
#include <QGraphicsItemGroup>
#include "globalinfo.h"
#include "interface/screen.h"
#include "help/pointoperators.h"
#include "troopinfo.h"
extern Screen * fullscreen;

extern QColor clear_c;

extern QPixmap * backpic;
extern QPixmap * highlightpic;

extern QList<QPixmap> selecticons;
extern QList<QPixmap> buildpics;
extern QList<QPixmap> trooppics;
extern QList<QPixmap> attachpics;

inline QPointF to_pointf(Point P){
    P *= BLOCKSIZE;
    return QPointF(P.X,P.Y);
}
inline QRectF to_squaref(Point P){
    P *= BLOCKSIZE;
    return QRectF(P.X,P.Y,BLOCKSIZE,BLOCKSIZE);
}

class BoardG:
    public QGraphicsItemGroup{
public:
    Point offp;
    BoardG(QGraphicsItem * parent=nullptr);
    void placepic(const QPixmap & pic,Point P);
    void create_background();
    void move_board(Point adj);
    Point screen_to_point(QPointF p){
        return board_to_point(p - this->pos());
    }
    Point board_to_point(QPointF p){
        p /= BLOCKSIZE;
        return Point{p.x(),p.y()};
    }
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};
class PlayerDomDisplay:
    public QGraphicsItemGroup{
public:
    //construct to create display, destruct to remove display
    PlayerDomDisplay(BoardG * boardg);
protected:
    void DrawColorSquare(QColor c, Point P, qreal opacity);
};

class MoneyG:
    public QGraphicsItemGroup{
public:
    QGraphicsTextItem * moneytext;
    MoneyG(QGraphicsItem * parent=nullptr);
    void setmoney(int value);
};

class MiniturnG:
    public QGraphicsItemGroup{
protected:
    QGraphicsRectItem * backs[3];
    static constexpr qreal fadeop = 0.5;
public:
    int miniturn;
    MiniturnG(QGraphicsItem * parent=nullptr);
    void makebox(const QString & str,const QFont & f,QColor color,int ypos);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

class BuildChooseG:
    public QGraphicsItemGroup{
protected:
    int buildpos = 0;
public:
    BuildChooseG(QGraphicsItem * parent=nullptr);
    void changeBuildPos(int amount);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

class SelectEmphasis:
        public QGraphicsItemGroup{
public:
    QList<QGraphicsItem *> sel_items;
    SelectEmphasis(QGraphicsItem * parent=nullptr);
    void clear_sel();
    void add_rect(Point up_left, Point bot_right, QColor color);
    void add_square(Point p, QColor color);
    void add_squares(vector<Point> squares,QColor color);
    void add_build_choice(SelectTypes choice, QColor color);
protected:
    void add_rect(QGraphicsItem * parent,QPointF up_l,QPointF size, QColor color);
};
