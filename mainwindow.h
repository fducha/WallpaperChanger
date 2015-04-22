#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QXmlStreamReader>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actAddWallpaper_triggered();
    void on_actRemoveWallpaper_triggered();

    void updateModel();
    void loadTrusty();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *wpModel;
    QStringList wpList;

    QXmlStreamReader reader;

    bool readXmlTrustyFile();

    void readXmlBackground();
    void readXmlStarttime();
    void readXmlStatic();
    void readXmlTransition();
    void readXmlUnknown();

    const QString TRUSTY_FILE_NAME = "/usr/share/backgrounds/contest/trusty.xml";
    int DURATION_STATIC;
    int DURATION_TRANS;
};

#endif // MAINWINDOW_H
