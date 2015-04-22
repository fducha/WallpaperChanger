#include <QFileDialog>
#include <QFileInfo>
#include <QModelIndex>
#include <QIcon>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    wpModel = 0;

    loadTrusty();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actAddWallpaper_triggered()
{
    QStringList wpNames = QFileDialog::getOpenFileNames(this, tr("Add new WallPapers"), "", "*.jpg");

    if (wpNames.isEmpty())
        return;

    wpList.append(wpNames);

    updateModel();
}

void MainWindow::updateModel()
{
    if (wpModel) {
        delete wpModel;
        wpModel = 0;
    }

    wpModel = new QStandardItemModel(wpList.count(), 1);


    for (int i = 0; i < wpModel->rowCount(); ++i) {
        QModelIndex index = wpModel->index(i, 0);
        QString wp = wpList.at(i);
        QFileInfo fi(wp);
        wpModel->setData(index, fi.baseName(), Qt::DisplayRole);
        wpModel->setData(index, QIcon(wp), Qt::DecorationRole);
    }

    ui->lvwWallPapers->setIconSize(QSize(160, 120));
    ui->lvwWallPapers->setViewMode(QListView::ListMode);
    ui->lvwWallPapers->setModel(wpModel);
}

void MainWindow::loadTrusty()
{
    wpList.clear();
    if (readXmlTrustyFile()) {
        updateModel();
    }
}

bool MainWindow::readXmlTrustyFile()
{
    QFile file(TRUSTY_FILE_NAME);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    reader.setDevice(&file);

    reader.readNext();

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            QString name = reader.name().toString();
            if (reader.name() == "background") {
                // read xml-trusty file
                readXmlBackground();
            } else {
                reader.raiseError(tr("Not a background-file."));
            }
        } else {
            reader.readNext();
        }
    }

    file.close();

    if (reader.hasError())
        return false;

    return true;
}

void MainWindow::readXmlBackground()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            QString name = reader.name().toString();
            if (reader.name() == "starttime") {
                // read starttime
                readXmlStarttime();
            } else if (reader.name() == "static") {
                // read static
                readXmlStatic();
            } else if (reader.name() == "transition") {
                //read transition
                readXmlTransition();
            } else {
                // read unknown element
                readXmlUnknown();
            }
        } else {
            reader.readNext();
        }
    }
}

void MainWindow::readXmlStarttime()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            QString name = reader.name().toString();
            if (reader.name() == "year") {
                // read year
                QString year = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "month") {
                // read month
                QString month = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "day") {
                // read day
                QString day = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "hour") {
                // read hour
                QString hour = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "minute") {
                // read minute
                QString minute = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "second") {
                // read second
                QString second = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            }
        } else {
            reader.readNext();
        }
    }
}

void MainWindow::readXmlStatic()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            QString name = reader.name().toString();
            if (reader.name() == "duration") {
                // read duration
                DURATION_STATIC = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "file") {
                QString fn = reader.readElementText();
                wpList.append(fn);
                if (reader.isEndElement())
                    reader.readNext();
            }
        } else {
            reader.readNext();
        }
    }
}

void MainWindow::readXmlTransition()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            QString name = reader.name().toString();
            if (reader.name() == "duration") {
                // read duration
                DURATION_TRANS = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "from") {
                QString fnFrom = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "to") {
                QString fnTo = reader.readElementText();
                if (reader.isEndElement())
                    reader.readNext();
            }
        } else {
            reader.readNext();
        }
    }
}

void MainWindow::readXmlUnknown()
{
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            readXmlUnknown();
        } else {
            reader.readNext();
        }
    }
}

void MainWindow::on_actRemoveWallpaper_triggered()
{
    if (ui->lvwWallPapers->currentIndex().isValid()) {
        int row = ui->lvwWallPapers->currentIndex().row();
        int r = QMessageBox::information(this, tr("Remove WallPaper"),
                                         tr("Do you really want to delete the selected wallpaper?"),
                                         QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::Yes) {
            wpList.removeAt(row);
            updateModel();
        }
    }
}
