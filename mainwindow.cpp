#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QModelIndex>
#include <QIcon>
#include <QMessageBox>
#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    durationStatic(1)
{
    ui->setupUi(this);

    wpModel = 0;
    durationTrans = 3;

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
        ui->sbxDuration->setValue(durationStatic);
    }
}

bool MainWindow::readXmlTrustyFile()
{
    QString fn = set.homeDir() + "/trusty.xml";
    QFile file(fn);

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

    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            QDate d(year, month, day);
            QTime t(hour, minute, second);
            starttime.setDate(d);
            starttime.setTime(t);
            break;
        }
        if (reader.isStartElement()) {
            //QString name = reader.name().toString();
            if (reader.name() == "year") {
                // read year
                year = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "month") {
                // read month
                month = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "day") {
                // read day
                day = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "hour") {
                // read hour
                hour = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "minute") {
                // read minute
                minute = reader.readElementText().toInt();
                if (reader.isEndElement())
                    reader.readNext();
            } else if (reader.name() == "second") {
                // read second
                second = reader.readElementText().toInt();
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
                QString dur = reader.readElementText();
                durationStatic = dur.left(dur.count() - 2).toInt() / 60;
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
                //durationTrans =
                reader.readElementText().toInt();
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

bool MainWindow::writeXmlTrustyFile(const QString &fileName)
{
    if (wpList.isEmpty())
        return false;

    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    writer.setDevice(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("background");

    writeXmlStarttime();
    // for-loop write static, transition
    for (int i = 0; i < wpList.count(); i++) {
        writeXmlStatic(wpList.at(i));
        if (i == wpList.count() - 1) {
            writeXmlTransition(wpList.at(i), wpList.at(0));
        } else {
            writeXmlTransition(wpList.at(i), wpList.at(i + 1));
        }
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();

    return true;
}

void MainWindow::writeXmlStarttime()
{
    writer.writeStartElement("starttime");

    if (!starttime.isValid()) {
        starttime.setDate(QDate::currentDate());
        starttime.setTime(QTime::currentTime());
    }

    writer.writeTextElement("year", numberAndZero(starttime.date().year()));
    writer.writeTextElement("month", numberAndZero(starttime.date().month()));
    writer.writeTextElement("day", numberAndZero(starttime.date().day()));
    writer.writeTextElement("hour", numberAndZero(starttime.time().hour()));
    writer.writeTextElement("minute", numberAndZero(starttime.time().minute()));
    writer.writeTextElement("second", numberAndZero(starttime.time().second()));

    writer.writeEndElement();
}

void MainWindow::writeXmlStatic(const QString &fileName)
{
    writer.writeStartElement("static");

    writer.writeTextElement("duration", QString("%1.0").arg(durationStatic * 60));
    writer.writeTextElement("file", fileName);

    writer.writeEndElement();
}

void MainWindow::writeXmlTransition(const QString &fnFrom, const QString &fnTo)
{
    writer.writeStartElement("transition");

    writer.writeTextElement("duration", QString("%1.0").arg(durationTrans));
    writer.writeTextElement("from", fnFrom);
    writer.writeTextElement("to", fnTo);

    writer.writeEndElement();
}

bool MainWindow::launchProcessSetWallPaperSet()
{
    QProcess pr;
    QString app = "gsettings";
    QStringList args;
    args.append("set");
    args.append("org.gnome.desktop.background");
    args.append("picture-uri");
    args.append(QString("'file://%1/trusty.xml'").arg(set.homeDir()));
    pr.start(app, args);
    return pr.waitForFinished();
}

QString MainWindow::numberAndZero(int num)
{
    return num < 10 ? QString("0%1").arg(num) : QString::number(num);
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

void MainWindow::on_actCloseApp_triggered()
{
    close();
}

void MainWindow::on_actApplyChanges_triggered()
{
    if (set.homeDir().isEmpty()) {
        //QMessageBox::information(this, "", QDir::homePath());
        QDir::home().mkdir(".WallPaperChanger");
        set.setHomeDir(QDir::homePath() + "/.WallPaperChanger");
    }
    writeXmlTrustyFile(set.homeDir() + "/" +"trusty.xml");
    launchProcessSetWallPaperSet();
}

void MainWindow::on_sbxDuration_valueChanged(int value)
{
    durationStatic = value;
}
