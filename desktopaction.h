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

#ifndef DESKTOPACTION_H
#define DESKTOPACTION_H

#include <QString>
#include <QStringList>

#include "action.h"
#include "btree.h"

class DesktopAction: public Action {
public:
    DesktopAction(QString file,QObject * parent=NULL);
    static void LoadDesktopActions(BTree*, QObject* parent=NULL);
    static QStringList* GetPaths();
public slots:
    virtual void runAction();
    virtual QString getIcon();
    bool mustShow();
private:
    bool show;
    bool terminal;
    bool cached_icon = false;
    QString cached_icon_path;

    void clear_action();
};



#endif // DESKTOPACTION_H
