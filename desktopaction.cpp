#include <QSettings>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>

#include "desktopaction.h"
#include "iconfinder.h"

/**
 * @brief DesktopAction::clear_action
 *
 * Removes the placeholders for the command line parameters.
 */
void DesktopAction::clear_action() {
    static const char * substitutions[]  = {
        "%f",
        "%F",
        "%u",
        "%U",
        "%d",
        "%D",
        "%n",
        "%N",
        "%i",
        "%c",
        "%k",
        "%v",
        "%m",
    };

    for (unsigned int i=0; i< sizeof(substitutions)/sizeof(int*); i++) {
        this->action = this->action.replace(substitutions[i],"");
    }
}

DesktopAction::DesktopAction(QString file, QObject *parent): Action(parent) {
    QSettings settings(file, QSettings::IniFormat);
    this->name = settings.value("Desktop Entry/Name","").toString();
    this->icon = settings.value("Desktop Entry/Icon","").toString();
    this->action = settings.value("Desktop Entry/Exec","").toString();
    this->terminal = settings.value("Desktop Entry/Terminal","false").toBool();

    this->clear_action();
}

void DesktopAction::runAction() {
    printf("%s\n", this->action.toStdString().c_str());
    QProcess::startDetached(this->action);
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
