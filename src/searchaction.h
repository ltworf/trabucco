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
*/

#ifndef SEARCHACTION_H
#define SEARCHACTION_H

#include <QString>
#include <QStringList>
#include <QList>

#include "action.h"
#include "btree.h"

class SearchAction: public Action {
public:
    static QList<SearchAction*> LoadFile(QString file, QObject* parent=NULL);
    static void LoadSearchActions(BTree*, QObject* parent=NULL);
    static QStringList* GetPaths();
    bool mustShow();
public slots:
    virtual bool isPrefix();
    virtual QString getIcon();
    virtual void runAction(QString);
    virtual bool hasCornerIcon();
    virtual QString getCornerIcon();
private:
    SearchAction(QString name, QString query, bool hidden, QObject* parent=NULL);
    QString query;
    QString hidden;
    bool show;
};

#endif // SEARCHACTION_H

