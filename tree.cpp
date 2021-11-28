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

#include "tree.h"

#include <QSettings>

#include "btree.h"
#include "btreeiterator.h"
#include "bookmarkaction.h"
#include "searchaction.h"

Tree::Tree(QObject *parent) : QObject(parent) {
    QSettings settings;
    this->bookmarks = settings.value("Source/Bookmarks", true).toBool();
    this->desktop = settings.value("Source/Desktop", true).toBool();
    this->searchprovider = settings.value("Source/SearchProvider", true).toBool();

    if(this->desktop)
        this->watcher.addPaths(*DesktopAction::GetPaths());
    if(this->searchprovider)
        this->watcher.addPaths(*SearchAction::GetPaths());
    if(this->bookmarks)
        this->watcher.addPaths(*BookmarkAction::GetPaths());

    connect(
        &this->watcher,
        &QFileSystemWatcher::directoryChanged,
        this,
        &Tree::rescan
    );
    connect(
        &this->watcher,
        &QFileSystemWatcher::fileChanged,
        this,
        &Tree::rescan
    );

    rescan();
}

void Tree::rescan() {
    this->ready = false;

    printf("Rescan triggered\n");

    {
        delete this->action_parent;
        delete this->node;
    }

    {
        this->action_parent = new QObject(this);
        this->node = new Node();
        BTree sorted_tree;
        if (this->desktop)
            DesktopAction::LoadDesktopActions(&sorted_tree, this->action_parent);
        if (this->bookmarks)
            BookmarkAction::LoadBookmarkActions(&sorted_tree, this->action_parent);
        if (this->searchprovider)
            SearchAction::LoadSearchActions(&sorted_tree, this->action_parent);
        BTreeIterator i(&sorted_tree);

        while (i.hasNext()) {
            Action* a = i.next();
            node->add(a);
        }
    }

    this->ready = true;
}

void Tree::runAction() {
    if (this->last == NULL || !this->ready)
        return;
    if (last->isPrefix()) {
        QString query = this->last_prefix.mid(last->getName().size());
        last->runAction(query);
    } else
        last->runAction();
}

Action * Tree::search(QString prefix) {
    if (!this->ready)
        return NULL;
    printf("Searching %s\n", prefix.toStdString().c_str());
    this->last_prefix = prefix;
    return this->last = this->node->search(prefix);
}
