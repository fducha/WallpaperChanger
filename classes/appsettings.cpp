#include <QSettings>
#include <QDir>

#include "appsettings.h"

AppSettings::AppSettings() :
    company("fducha Inc."),
    appName("Wallpaper Changer")
{
}

AppSettings::~AppSettings()
{

}

void AppSettings::setHomeDir(const QString &dir)
{
    QDir d(dir);
    if (!d.exists())
        return;
    QSettings set(company, appName);
    set.setValue("HomeDirectory", dir);
}

QString AppSettings::homeDir() const
{
    QSettings set(company, appName);
    return set.value("HomeDirectory", "").toString();
}

