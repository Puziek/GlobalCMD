#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{

}

void SettingsManager::putSetting(QString group, QString key, QVariant value)
{
    QSettings settings;
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
    settings.sync();
}

QVariant SettingsManager::getSetting(QString group, QString key, QVariant defaultValue)
{
    QSettings settings;
    settings.beginGroup(group);
    QVariant value = settings.value(key, defaultValue);
    settings.endGroup();

    return value;
}
