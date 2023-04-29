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

Copyright (C) 2023  Salvo "LtWorf" Tomaselli
*/

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>

#include "iconfinder.h"
#include "passaction.h"

static QString separator;

PassAction::PassAction(QString script, QString prefix, QObject *parent) : Action(parent)
{
    this->script = script;
    this->name = prefix + separator + script;
}

QString PassAction::getIcon() {
    return this->cached_icon_path;
}

void PassAction::runAction() {
    QStringList arguments;
    arguments << script;
    QProcess proc;
    proc.setProgram("pass");
    proc.setArguments(arguments);
    proc.startDetached();
}

#include <QDebug>

void PassAction::scanAndLoad(BTree* tree, QObject* parent, QString base, QString icon, QStringList prefixes, unsigned int lchop) {
    QDirIterator i(base, QDirIterator::FollowSymlinks);
    while (i.hasNext()) {
        QFileInfo file(i.next());

        if (file.fileName().startsWith("."))
            continue;
        if (file.isDir() && file.isExecutable()) {
            PassAction::scanAndLoad(tree, parent, file.absoluteFilePath(), icon, prefixes, lchop);
        }
        if (!file.fileName().endsWith(".gpg"))
            continue;

        for (int i = 0; i < prefixes.size(); i++) {
            QString name = file.absoluteFilePath();
            // Remove extension
            name.chop(4);
            // Remove initial path
            name.remove(0, lchop);

            qDebug() << "[pass] adding " << prefixes.at(i) << name << base;
            PassAction* pass_action = new PassAction(name, prefixes.at(i), parent);
            pass_action->cached_icon_path = icon;
            tree->add(pass_action);
        }
    }
}

void PassAction::LoadPassActions(BTree* tree, QObject* parent) {
    if (!PassAction::isPassInstalled()) {
        return;
    }
    QStringList* dirs = PassAction::GetPaths();
    QString dir = dirs->at(0);

    QSettings settings;
    separator = settings.value("PassAction/separator", " ").toString();

    QString icon = IconFinder::FindIcon("copy");
    QStringList prefixes = settings.value("PassAction/prefixes", "pass").toStringList();

    PassAction::scanAndLoad(tree, parent, dir, icon, prefixes, dir.size());
}


QStringList* PassAction::GetPaths() {
    static QStringList* result = NULL;
    if (result)
        return result;

    result = new QStringList();
    result->append(QDir::homePath() + "/.password-store/");
    return result;
}

bool PassAction::isPassInstalled() {
    QFile pass("/usr/bin/pass");
    return pass.exists();
}
