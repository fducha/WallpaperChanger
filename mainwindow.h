#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDateTime>

#include "classes/appsettings.h"

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
    void on_actApplyChanges_triggered();
    void on_actCloseApp_triggered();

    void on_sbxDuration_valueChanged(int value);

    void updateModel();
    void loadTrusty();

    void on_actNewSet_triggered();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *wpModel;
    QStringList wpList;

    QDateTime starttime;

    QXmlStreamReader reader;
    QXmlStreamWriter writer;

    AppSettings set;

    bool readXmlTrustyFile();
    void readXmlBackground();
    void readXmlStarttime();
    void readXmlStatic();
    void readXmlTransition();
    void readXmlUnknown();

    bool writeXmlTrustyFile(const QString &fileName);
    void writeXmlStarttime();
    void writeXmlStatic(const QString &fileName);
    void writeXmlTransition(const QString &fnFrom, const QString &fnTo);

    bool launchProcessSetWallPaperSet();

    int durationStatic;
    int durationTrans;

    QString numberAndZero(int num);
};

#endif // MAINWINDOW_H
