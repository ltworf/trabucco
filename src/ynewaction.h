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

Copyright (C) 2017-2021  Salvo "LtWorf" Tomaselli
*/

#ifndef YNEWACTION_H
#define YNEWACTION_H

#include <QString>
#include <QStringList>

#include "action.h"
#include "btree.h"

class YnewAction: public Action {
public:
    YnewAction(QString script, QString prefix, QObject * parent=NULL);
    static void LoadYnewActions(BTree*, QObject* parent=NULL);
    static QStringList* GetPaths();
    static bool isYnewInstalled();
public slots:
    virtual void runAction();
    virtual QString getIcon();
//    bool mustShow();
private:
    QString script;
    QString cached_icon_path;
};

#endif // DESKTOPACTION_H
