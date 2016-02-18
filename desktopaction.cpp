#include <QSettings>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>

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
        "%k",
        "%v",
        "%m",
    };

    this->action = this->action.replace("%c", this->name);

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

        if (!path.endsWith(".desktop")) {
            continue;
        }

        QFileInfo info(path);

        if (info.isFile() && info.isReadable()) {
            DesktopAction * action = new DesktopAction(path);
            result->push(action);
        }

    }
}

// Following the freedesktop.org specification, we look for applications in the
// `applications` subdirectory of:
// $XDG_DATA_HOME (which defaults to `$HOME/.local/share`),
// and all components of $XDG_DATA_DIRS (a colon-separated list of paths
// that defaults to `/usr/local/share/:/usr/share/`)

static const QString xdg_data_dirs_default = "/usr/local/share/:/usr/share/";

QStack<DesktopAction*> DesktopAction::LoadDesktopActions() {
    QStack<DesktopAction*> result;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QStringList xdg_data_dirs =
	    env.value("XDG_DATA_DIRS", xdg_data_dirs_default).split(":");

    // put user's data dir before the system ones
    xdg_data_dirs.prepend(env.value("XDG_DATA_HOME", QDir::homePath() + "/.local/share/"));

    QStringListIterator i(xdg_data_dirs);

    while (i.hasNext()) {
	    iterate_dir(&result, i.next() + "/applications");
    }

    return result;
}
