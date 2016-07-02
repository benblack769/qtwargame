#pragma once
#include "globalinfo.h"
#ifdef Debug_Macro_Move
#include <QWidget>
#include <QGraphicsRectItem>
#include "computerplayer.h"

class QVBoxLayout;
class QSpinBox;
class QRadioButton;
class SimpleCompPlayer;
class BoardG;
class QPushButton;
class QLabel;

class DebugInter : public QWidget
{
    Q_OBJECT
protected:
    static constexpr size_t num_ch = 4;
    static constexpr size_t num_ch_m = 3;
    QVBoxLayout * lay;
    QLabel * choicelabs[num_ch];
    QSpinBox * choices[num_ch];
    QRadioButton * choices_main[num_ch_m];
    QPushButton * redraw_but;
    QVector<QGraphicsRectItem *> qrects;
    QLabel * max_val_lab;
    QLabel * max_val_dis;
public:
    SimpleCompPlayer * compplay;

    explicit DebugInter(QWidget *parent = 0);
protected:
    void init_layout();
    void DrawDebugData(Array2d<double> & Data);
    void DrawColorSquare(QColor c, Point P, qreal opacity);
    void DrawMacroMoveStuff(vector<DArray2d<TroopInfo<Array2d<double>>>> InData[4], int Spots[4]);
    void DrawMacroMoveBuild(DArray2d<Array2d<double>> & BData, int Spots[2]);
    void DrawMicroMoveStuff(vector<vector<MoveSquareVals>> & MData, int Spots[4]);
    void clear_qrects();
    //void clear_text();
signals:

public slots:
    void print_stuff();
    void toggle_micromoves(bool clicked);
    void toggle_macromoves(bool clicked);
    void toggle_macrobuild(bool clicked);
};
#endif
