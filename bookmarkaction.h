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

#ifndef BOOKMARKACTION_H
#define BOOKMARKACTION_H

#include <QString>
#include <QUrl>
#include <QStringList>

#include "action.h"
#include "btree.h"

class BookmarkAction: public Action {
public:
    BookmarkAction(QString name, QUrl url, QString icon, QObject* parent=NULL);
    static void LoadBookmarkActions(BTree*, QObject* parent=NULL);
    static QStringList* GetPaths();
public slots:
    virtual void runAction();
    virtual QString getIcon();
    virtual bool hasCornerIcon();
    virtual QString getCornerIcon();
private:
    QUrl url;
};

#endif // BOOKMARKACTION_H
