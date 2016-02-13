#include <QSettings>

#include "desktopaction.h"

DesktopAction::DesktopAction(QString file, QObject *parent): Action(parent) {
    QSettings settings(file, QSettings::IniFormat);
    this->name = settings.value("Desktop Entry/Name","").toString();
    this->icon = settings.value("Desktop Entry/Icon","").toString();
    this->action = settings.value("Desktop Entry/Exec","").toString();
    this->terminal = settings.value("Desktop Entry/Terminal","").toBool();
}

void DesktopAction::runAction() {
    printf("%s\n", this->action.toStdString().c_str());
}
