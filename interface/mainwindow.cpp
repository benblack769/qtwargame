#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myview.h"
#include "screen.h"
#include "base.h"
#include <QHBoxLayout>
#include "debuginter.h"

void save1(){SaveGameInfo("SaveSlot1.bin");}
void save2(){SaveGameInfo("SaveSlot2.bin");}
void save3(){SaveGameInfo("SaveSlot3.bin");}

void load1(){LoadGameInfo("SaveSlot1.bin");}
void load2(){LoadGameInfo("SaveSlot2.bin");}
void load3(){LoadGameInfo("SaveSlot3.bin");}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    load_imgs();

    screen = new Screen;
    screen->setSceneRect(QRectF(0, 0, BoardSizeX*BLOCKSIZE, BoardSizeY*BLOCKSIZE));
    veiw = new MyView(screen);

#ifdef Debug_Macro_Move
    deb = new DebugInter(this);

    QHBoxLayout * lay = new QHBoxLayout(this);
    lay->addWidget(veiw,100,Qt::AlignLeft);
    lay->addWidget(deb);
    QWidget * W = new QWidget;
    W->setLayout(lay);
    setCentralWidget(W);
#else
    setCentralWidget(veiw);
#endif
    setWindowTitle(tr("Battlefeild"));

    connect(ui->actionSave1,&QAction::triggered, save1);
    connect(ui->actionSave2,&QAction::triggered, save2);
    connect(ui->actionSave3,&QAction::triggered, save3);

    connect(ui->actionLoad1,&QAction::triggered, load1);
    connect(ui->actionLoad2,&QAction::triggered, load2);
    connect(ui->actionLoad3,&QAction::triggered, load3);
}
MainWindow::~MainWindow()
{
    delete ui;
}
