#include "myview.h"
#include "interface/screen.h"

MyView::MyView(QGraphicsScene * scene):
    QGraphicsView(scene),
    myscene(reinterpret_cast<Screen * >(scene))
{
    QAbstractScrollArea::setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    QAbstractScrollArea::setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}
void MyView::resizeEvent(QResizeEvent *event)
{
    QRect rcontent = this->contentsRect();
    this->setSceneRect(0, 0, rcontent.width(), rcontent.height());
    //fitInView(0, 0, BoardSizeX*BLOCKSIZE, BoardSizeY*BLOCKSIZE,Qt::KeepAspectRatio);
    this->myscene->resize(rcontent);
    QGraphicsView::resizeEvent(event);
}
