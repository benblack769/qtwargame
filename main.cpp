#include "interface/mainwindow.h"
#include <QApplication>
#include "player.h"
#include "base.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#ifdef Debug_Macro_Move
    InitiateGame(w.deb);
#else
    InitiateGame(nullptr);
#endif
    w.show();

    return a.exec();
}
