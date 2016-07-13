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
    for(int i : range(NUM_VARS)){
        lay->addWidget(choicelabs[i],0,Qt::AlignTop);
        lay->addWidget(choices[i],0,Qt::AlignTop);
    }

    lay->addWidget(redraw_but,0,Qt::AlignTop);

    lay->addWidget(max_val_lab,0,Qt::AlignTop);
    lay->addWidget(max_val_dis,1,Qt::AlignTop);
}
void DebugInter::make_unused(){
    for(QLabel * cl : choicelabs)
        cl->setText(QString("unused"));
}
void DebugInter::toggle_micromoves(bool clicked){
    if(clicked){
        make_unused();
        choicelabs[0]->setText(QString("num of iters"));
        choicelabs[1]->setText(QString("num of troops"));
        choicelabs[2]->setText(QString("move location/attackinfo/moveinfo/otherstuff"));
    }
}
void DebugInter::toggle_macromoves(bool clicked){
    if(clicked){
        make_unused();
        choicelabs[0]->setText(QString("Vals/probs"));
        choicelabs[1]->setText(QString("iteration"));
        choicelabs[2]->setText(QString("move num"));
        choicelabs[3]->setText(QString("player num"));
        choicelabs[4]->setText(QString("build/troop"));
        choicelabs[5]->setText(QString("bt num"));
    }
}
void DebugInter::toggle_macrobuild(bool clicked){
    if(clicked){
        make_unused();
        choicelabs[0]->setText(QString("num of iters"));
        choicelabs[1]->setText(QString("move num"));
    }
}
void DebugInter::print_stuff(){
    clear_qrects();
    int ch_ints[NUM_VARS];
    for(int i : range(NUM_VARS)){
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

    max_val_dis->setText(QString(to_string(MaxD).c_str()));

    if(MaxD == 0)
        return;

    for (Point P : BoardIterate()){
        double D = Data[P];
        QColor C = D > 0 ? Qt::blue : Qt::black;
        DrawColorSquare(C, P, abs(D) / MaxD);
    }
}
void DebugInter::DrawMacroMoveStuff(macro_debug_info & InData, int Spots[5]){
    if(Spots[0] >= 2)
        return;
    
    vector<vector<vector<TBVals>>> & rel_data = *(InData.probvals+Spots[0]);
    if(Spots[1] >= rel_data.size())
        return;
    
    vector<vector<TBVals>> & iter_data = rel_data[Spots[1]];
    if(Spots[2] >= iter_data.size())
        return;
    
    vector<TBVals> & move_data = iter_data[Spots[2]];
    if(Spots[3] >= move_data.size())
        return;
    
    TBVals & tb_data = move_data[Spots[3]];
    if(Spots[4] >= 2)
        return;
    
    vector<Array2d<double>> & t_or_b_data = *(Spots[4] == 0 ? &(tb_data.troopvs) : &(tb_data.buildvs));
    
    if(Spots[5] >= t_or_b_data.size())
        return;
    
    DrawDebugData(t_or_b_data[Spots[5]]);
}
void DebugInter::DrawMacroMoveBuild(DArray2d<Array2d<double>> & BData, int Spots[2]){
    if (BData.Data.size() == 0)
        return;
    int Sizes[2] = { BData.dim1(), BData.dim2() };

    if (IsInScope(Sizes,Spots,2)){
        DrawDebugData(BData[Spots[0]][Spots[1]]);
    }
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
}
#endif
