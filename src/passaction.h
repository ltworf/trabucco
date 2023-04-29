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

#ifndef PASSACTION_H
#define PASSACTION_H

#include <QString>
#include <QStringList>

#include "action.h"
#include "btree.h"

class PassAction: public Action {
public:
    PassAction(QString script, QString prefix, QObject * parent=NULL);
    static void LoadPassActions(BTree*, QObject* parent=NULL);
    static QStringList* GetPaths();
    static bool isPassInstalled();
public slots:
    virtual void runAction();
    virtual QString getIcon();
//    bool mustShow();
private:
    QString script;
    QString cached_icon_path;
    static void scanAndLoad(BTree* tree, QObject* parent, QString base, QString icon, QStringList prefixes, unsigned int lchop);
};

#endif // PASSACTION_H
