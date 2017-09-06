/*
This file is part of Trabucco.

Trabucco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Trabucco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Trabucco.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2016  Salvo "LtWorf" Tomaselli
Copyright (C) 2016 Giuseppe Bilotta
*/

#include <QSettings>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>
#include <QSet>

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
    settings.setIniCodec("UTF-8");
    this->name = settings.value("Desktop Entry/Name","").toString();
    this->icon = settings.value("Desktop Entry/Icon","").toString();
    this->action = settings.value("Desktop Entry/Exec","").toString();
    this->terminal = settings.value("Desktop Entry/Terminal","false").toBool();

    this->clear_action();

    this->show = true;

    //Determine wheter to show it
    if (settings.value("Desktop Entry/NoDisplay","false").toBool()) {
        this->show = false;
    }

    //Show/hide depending on the DE being used
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QSet<QString> desktop_environments = QSet<QString>::fromList(
            env.value("XDG_CURRENT_DESKTOP","").split(":")
                                         );
    QSet<QString> not_show = QSet<QString>::fromList(
                                 settings.value("Desktop Entry/NotShowIn","").toString().split(":")
                             );
    QSet<QString> show_in = QSet<QString>::fromList(
                                settings.value("Desktop Entry/OnlyShowIn","").toString().split(":")
                            );

    desktop_environments.remove("");
    not_show.remove("");
    show_in.remove("");

    int show_in_c = show_in.size();

    if (not_show.size()) {
        not_show.intersect(desktop_environments);
        if (not_show.size() != 0) {
            this->show = false;
        }
    }

    if (show_in_c) {
        show_in.subtract(desktop_environments);
        if (show_in_c == show_in.size()) {
            this->show = false;
        }
    }

    if (settings.value("Desktop Entry/Hidden", "false").toBool()) {
        this->show = false;
    }
}

bool DesktopAction::mustShow() {
    return this->show;
}

void DesktopAction::runAction() {
    printf("%s\n", this->action.toStdString().c_str());
    if (!this->terminal) {
        QProcess::startDetached(this->action);
    } else {
        QProcess::startDetached("x-terminal-emulator -e " + this->action);
    }

}

QString DesktopAction::getIcon() {
    if (!this->cached_icon) {
        this->cached_icon_path = IconFinder::FindIcon(this->icon);
        this->cached_icon = true;
    }
    return this->cached_icon_path;
}

static void iterate_dir(BTree* tree, QString dir, QObject* parent) {

    // Include subdirectories and follow links
    QDirIterator i(dir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

    while (i.hasNext()) {
        QString path = i.next();

        if (!path.endsWith(".desktop")) {
            continue;
        }

        QFileInfo info(path);

        if (info.isFile() && info.isReadable()) {
            DesktopAction * action = new DesktopAction(path, parent);
            if (!action->mustShow() || !tree->add(action))
                delete action;
        }

    }
}

// Following the freedesktop.org specification, we look for applications in the
// `applications` subdirectory of:
// $XDG_DATA_HOME (which defaults to `$HOME/.local/share`),
// and all components of $XDG_DATA_DIRS (a colon-separated list of paths
// that defaults to `/usr/local/share/:/usr/share/`)

static const QString xdg_data_dirs_default = "/usr/local/share/:/usr/share/";

/**
 * @brief DesktopAction::GetPaths
 * @return the list of paths containing .desktop files
 */
QStringList* DesktopAction::GetPaths() {
    static QStringList* result = NULL;
    if (result)
        return result;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QStringList xdg_data_dirs =
        env.value("XDG_DATA_DIRS", xdg_data_dirs_default).split(":");

    // put user's data dir before the system ones
    xdg_data_dirs.prepend(env.value("XDG_DATA_HOME", QDir::homePath() + "/.local/share/"));

    result = new QStringList();
    QStringListIterator i(xdg_data_dirs);
    while (i.hasNext()) {
        result->append(i.next() + "/applications");
    }
    result->removeDuplicates();

    return result;
}

/**
 * @brief DesktopAction::LoadDesktopActions
 * @return a stack containing Action for all the .desktop files
 */
void DesktopAction::LoadDesktopActions(BTree* tree, QObject *parent) {
    QStringListIterator i(*DesktopAction::GetPaths());

    while (i.hasNext()) {
        iterate_dir(tree, i.next(), parent);
    }
}
