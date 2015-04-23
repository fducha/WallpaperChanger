#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/icons/preferences-desktop-wallpaper_5449.png"));
    MainWindow w;
    w.show();

    return a.exec();
}
