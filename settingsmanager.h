#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);
    static void putSetting(QString group, QString key, QVariant value);
    static QVariant getSetting(QString group, QString key, QVariant defaultValue);

signals:

public slots:
};

#endif // SETTINGSMANAGER_H
