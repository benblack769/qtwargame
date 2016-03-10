#pragma once
#include <QGraphicsView>

class Screen;
class MyView : public QGraphicsView
{
public:
    MyView(QGraphicsScene *scene);

protected:
    void resizeEvent(QResizeEvent *event);
    Screen * myscene;
};
