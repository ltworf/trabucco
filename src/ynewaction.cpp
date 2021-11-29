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

Copyright (C) 2017  Salvo "LtWorf" Tomaselli
*/

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>

#include "iconfinder.h"
#include "ynewaction.h"

static QString separator;

YnewAction::YnewAction(QString script, QString prefix, QObject *parent) : Action(parent)
{
    this->script = script;
    this->name = prefix + separator + script;
}

QString YnewAction::getIcon() {
    return this->cached_icon_path;
}

void YnewAction::runAction() {
    QStringList arguments;
    arguments << script;
    QProcess::startDetached("ynew", arguments);
}

void YnewAction::LoadYnewActions(BTree* tree, QObject* parent) {
    QStringList* dirs = YnewAction::GetPaths();
    QString dir = dirs->at(0);
    QSettings settings;
    QStringList prefixes = settings.value("YnewAction/prefixes", "ynew").toStringList();
    separator = settings.value("YnewAction/separator", " ").toString();
    QString yakuake_icon = IconFinder::FindIcon("yakuake");

    QDirIterator i(dir, QDirIterator::FollowSymlinks);
    while (i.hasNext()) {
        QFileInfo file(i.next());
        if (file.isDir() || (! file.isExecutable()))
            continue;

        for (int i = 0; i < prefixes.size(); i++) {
            YnewAction* ynew_action = new YnewAction(file.fileName(), prefixes.at(i), parent);
            ynew_action->cached_icon_path = yakuake_icon;
            tree->add(ynew_action);
        }
    }
}

QStringList* YnewAction::GetPaths() {
    static QStringList* result = NULL;
    if (result)
        return result;

    result = new QStringList();
    result->append(QDir::homePath() + "/.ynew/");
    return result;
}
