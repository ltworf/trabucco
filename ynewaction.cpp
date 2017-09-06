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

#include "iconfinder.h"
#include "ynewaction.h"

YnewAction::YnewAction(QString script, QObject *parent) : Action(parent)
{
    this->script = script;
    this->cached_icon_path = IconFinder::FindIcon("yakuake");
    this->name = "ynew " + script;
}

QString YnewAction::getIcon() {
    return this->cached_icon_path;
}

void YnewAction::runAction() {
    QProcess::startDetached("ynew " + script);
}

void YnewAction::LoadYnewActions(BTree* tree, QObject* parent) {
    QStringList* dirs = YnewAction::GetPaths();
    QString dir = dirs->at(0);

    QDirIterator i(dir, QDirIterator::FollowSymlinks);
    while (i.hasNext()) {
        QFileInfo file(i.next());
        if (file.isDir() || (! file.isExecutable()))
            continue;
        tree->add(new YnewAction(file.fileName(), parent));
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
