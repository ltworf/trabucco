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

Copyright (C) 2016-2023  Salvo "LtWorf" Tomaselli
Copyright (C) 2016 Giuseppe Bilotta
*/

#ifndef TREE_H
#define TREE_H

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>

#include "desktopaction.h"
#include "action.h"
#include "node.h"

class Tree : public QObject {
    Q_OBJECT
public:
    explicit Tree(QObject *parent = 0);

public slots:
    Action * search(QString prefix);
    void runAction();
    void rescan();
private:
    bool ready = false;
    bool bookmarks;
    bool desktop;
    bool searchprovider;
    bool ynew;
    bool pass;
    QObject* action_parent = NULL;
    Action * last = NULL;
    QString last_prefix;
    Node * node = NULL;
    QFileSystemWatcher watcher;
};

#endif // TREE_H
