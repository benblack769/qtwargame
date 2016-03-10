#pragma once
#include <QMainWindow>
#include "globalinfo.h"

class MyView;
namespace Ui {
class MainWindow;
}
class Screen;
class MenuInterface;
class DebugInter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

#ifdef Debug_Macro_Move
    DebugInter * deb;
#endif
protected:
    Ui::MainWindow *ui;
    MyView * veiw;
    Screen * screen;
};
