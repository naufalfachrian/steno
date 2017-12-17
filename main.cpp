#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    QDesktopWidget wid;

    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();
    int x = (screenWidth / 2) - w.width() / 2;
    int y = (screenHeight / 2) - w.height() / 2;

    w.setGeometry(x, y, w.width(), w.height());

    w.show();

    return a.exec();
}
