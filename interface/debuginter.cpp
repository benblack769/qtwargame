#include "debuginter.h"
#ifdef Debug_Macro_Move
#include <QVBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QGraphicsRectItem>
#include "interface/screenhelp.h"
#include "headerlib/RangeIterator.h"
#include "computerplayer.h"


DebugInter::DebugInter(QWidget *parent) :
    QWidget(parent){
    lay = new QVBoxLayout(this);
    for (QSpinBox *& c : choices)
        c = new QSpinBox(this);
    for(QLabel *& cl : choicelabs)
        cl = new QLabel(this);

    choices_main[0] = new QRadioButton(tr("micro moves"),this);
    choices_main[1] = new QRadioButton(tr("macro build moves"),this);
    choices_main[2] = new QRadioButton(tr("macro troop moves"),this);

    connect(choices_main[0],&QRadioButton::toggled,this,&DebugInter::toggle_micromoves);
    connect(choices_main[1],&QRadioButton::toggled,this,&DebugInter::toggle_macrobuild);
    connect(choices_main[2],&QRadioButton::toggled,this,&DebugInter::toggle_macromoves);

    redraw_but = new QPushButton("reset",this);
    connect(redraw_but,&QPushButton::clicked,this,&DebugInter::print_stuff);

    max_val_lab = new QLabel(tr("max of displayed values"),this);
    max_val_dis = new QLabel(tr("filler"),this);

    init_layout();
    this->setLayout(lay);
}
void DebugInter::init_layout(){
    for (QRadioButton * cm : choices_main)
        lay->addWidget(cm,0,Qt::AlignTop);
    //breaking it up necessary to get it all at the top like it is
    for(int i : range(num_ch)){
        lay->addWidget(choicelabs[i],0,Qt::AlignTop);
        lay->addWidget(choices[i],0,Qt::AlignTop);
    }

    lay->addWidget(redraw_but,0,Qt::AlignTop);

    lay->addWidget(max_val_lab,0,Qt::AlignTop);
    lay->addWidget(max_val_dis,1,Qt::AlignTop);
}
/*void DebugInter::clear_text(){
    for(QLabel * cl : choicelabs)
        cl->setText(QString());
}*/
void DebugInter::toggle_micromoves(bool clicked){
    if(clicked){
        choicelabs[0]->setText(QString("num of iters"));
        choicelabs[1]->setText(QString("num of troops"));
        choicelabs[2]->setText(QString("move location/attackinfo/moveinfo/otherstuff"));
        choicelabs[3]->setText(QString("unused"));
    }
}
void DebugInter::toggle_macromoves(bool clicked){
    if(clicked){
        choicelabs[0]->setText(QString("TP/NextTP/SpreadTPaths/RawTPaths"));
        choicelabs[1]->setText(QString("iteration"));
        choicelabs[2]->setText(QString("move num"));
        choicelabs[3]->setText(QString("troop num"));
    }
}
void DebugInter::toggle_macrobuild(bool clicked){
    if(clicked){
        choicelabs[0]->setText(QString("num of iters"));
        choicelabs[1]->setText(QString("move num"));
        choicelabs[2]->setText(QString("unused"));
        choicelabs[3]->setText(QString("unused"));
    }
}
void DebugInter::print_stuff(){
    clear_qrects();
    int ch_ints[num_ch];
    for(int i : range(num_ch)){
        ch_ints[i] = choices[i]->value();
    }
    if(choices_main[0]->isChecked())
        DrawMicroMoveStuff(compplay->VecMVals,ch_ints);
    else if(choices_main[1]->isChecked())
        DrawMacroMoveBuild(compplay->VecBVals,ch_ints);
    else if(choices_main[2]->isChecked())
        DrawMacroMoveStuff(compplay->VecTVals,ch_ints);
}
void DebugInter::DrawColorSquare(QColor c,Point P,qreal opacity){
    QGraphicsRectItem * ptr = new QGraphicsRectItem(to_squaref(P),fullscreen->board);
    ptr->setBrush(QBrush(c));
    ptr->setOpacity(opacity);
    ptr->setZValue(100);
    qrects.push_back(ptr);
}
void DebugInter::clear_qrects(){
    for (QGraphicsRectItem * ptr : qrects){
        delete ptr;
    }
    qrects.clear();
}
bool IsInScope(int Sizes[], int Spots[], int Size){
    for (int n : range(Size))
        if (Spots[n] < 0 || Spots[n] >= Sizes[n])
            return false;
    return true;
}
void DebugInter::DrawDebugData(Array2d<double> & Data){
    double MaxD = 0;
    for (double Info : Data)
        MaxD = max(MaxD, abs(Info));

    if(MaxD == 0)
        return;

    for (Point P : BoardIterate()){
        double D = Data[P];
        QColor C = D > 0 ? Qt::blue : Qt::black;
        DrawColorSquare(C, P, abs(D) / MaxD);
    }
    max_val_dis->setText(QString(to_string(MaxD).c_str()));
}
void DebugInter::DrawMacroMoveStuff(vector<DArray2d<TroopInfo<Array2d<double>>>> InData[4], int Spots[4]){
    if (InData[0].size() <= 0 || InData[0][0].dim1() <= 0 || InData[0][0][0].Size <= 0)
        return;
    int Sizes[4];
    Sizes[0] = 4;
    Sizes[1] = InData[0].size();
    Sizes[2] = InData[0][0].dim1();
    Sizes[3] = InData[0][0][0].Size;

    if (IsInScope(Sizes,Spots,4))
        DrawDebugData(InData[Spots[0]][Spots[1]][Spots[2]][Spots[3]].Info);

    //DrawPoints(Sizes,Spots,4);
}
void DebugInter::DrawMacroMoveBuild(DArray2d<Array2d<double>> & BData, int Spots[2]){
    if (BData.Data.size() == 0)
        return;
    int Sizes[2] = { BData.dim1(), BData.dim2() };

    if (IsInScope(Sizes,Spots,2)){
        DrawDebugData(BData[Spots[0]][Spots[1]]);
    }
    //DrawPoints(Sizes,Spots,2);
}
void DebugInter::DrawMicroMoveStuff(vector<vector<MoveSquareVals>> & MData, int Spots[4]){
    if (MData.size() == 0 || MData[0].size() == 0 || MData[0][0].Size() == 0)
        return;
    const int Size = 3;
    int Sizes[Size] = { MData.size(), MData[0].size(), MData[0][0].Size() + 3};
    if (IsInScope(Sizes, Spots, Size)){
        Array2d<double> Vals(0);
        if (Spots[2] == Sizes[2] - 1) {
            for (auto MPair : MData[Spots[0]][Spots[1]])
                Vals[MPair.P] = MPair.Info().MoveV + max(MPair.Info().AttackV.Arr);
        }
        else if (Spots[2] == Sizes[2] - 2){
            for (auto MPair : MData[Spots[0]][Spots[1]])
                Vals[MPair.P] = max(MPair.Info().AttackV.Arr);
        }
        else if (Spots[2] == Sizes[2] - 3) {
            for (auto MPair : MData[Spots[0]][Spots[1]])
                Vals[MPair.P] = MPair.Info().MoveV;
        }
        else{
            for (auto APair : MData[Spots[0]][Spots[1]].Arr[Spots[2]].AttackV)
                Vals[APair.P] = APair.Info();
        }
        DrawDebugData(Vals);
    }
   // DrawPoints(Sizes, Spots, Size);
}
#endif
