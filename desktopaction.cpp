#include <QSettings>
#include <QDirIterator>
#include <QFileInfo>

#include "desktopaction.h"
#include "iconfinder.h"

DesktopAction::DesktopAction(QString file, QObject *parent): Action(parent) {
    QSettings settings(file, QSettings::IniFormat);
    this->name = settings.value("Desktop Entry/Name","").toString();
    this->icon = settings.value("Desktop Entry/Icon","").toString();
    this->action = settings.value("Desktop Entry/Exec","").toString();
    this->terminal = settings.value("Desktop Entry/Terminal","false").toBool();
}

void DesktopAction::runAction() {
    printf("%s\n", this->action.toStdString().c_str());
}

QString DesktopAction::getIcon() {
    if (!this->cached_icon_path.length())
        this->cached_icon_path = IconFinder::FindIcon(this->icon);
    return this->cached_icon_path;
}

static void iterate_dir(QStack<DesktopAction*> * result, QString dir) {

    // Include subdirectories and follow links
    QDirIterator i(dir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

    while (i.hasNext()) {
        QString path = i.next();
        QFileInfo info(path);

        if (info.isFile() && info.isReadable()) {
            DesktopAction * action = new DesktopAction(path);
            result->push(action);
        }

    }
}

QStack<DesktopAction*> DesktopAction::LoadDesktopActions() {
    QStack<DesktopAction*> result;

    iterate_dir(&result,"/usr/share/applications/");
    //TODO add local directories


    return result;
}
