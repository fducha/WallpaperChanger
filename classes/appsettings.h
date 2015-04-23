#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>

class AppSettings
{
public:
    AppSettings();
    ~AppSettings();

    void setHomeDir(const QString &dir);
    QString homeDir() const;

private:
    const QString company;
    const QString appName;
};

#endif // APPSETTINGS_H
