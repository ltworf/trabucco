#include "tree.h"

#include <QStringListIterator>
#include <QSettings>

#include "btree.h"
#include "btreeiterator.h"
#include "bookmarkaction.h"
#include "searchaction.h"

Tree::Tree(QObject *parent) : QObject(parent) {
    this->watcher.addPaths(*DesktopAction::GetPaths());

    connect(
        &this->watcher,
        &QFileSystemWatcher::directoryChanged,
        this,
        &Tree::rescan
    );

    QSettings settings;
    this->bookmarks = settings.value("Source/Bookmarks", true).toBool();
    this->desktop = settings.value("Source/Desktop", true).toBool();
    this->searchprovider = settings.value("Source/SearchProvider", true).toBool();

    rescan();
}

void Tree::rescan() {
    this->ready = false;

    printf("Rescan triggered\n");

    {
        QListIterator<Action*> i(this->actions);
        while (i.hasNext()) {
            delete i.next();
        }
        this->actions.clear();

        delete this->node;
    }

    {
        this->node = new Node();
        BTree sorted_tree;
        if (this->desktop)
            DesktopAction::LoadDesktopActions(&sorted_tree);
        if (this->bookmarks)
            BookmarkAction::LoadBookmarkActions(&sorted_tree);
        if (this->searchprovider)
            SearchAction::LoadSearchActions(&sorted_tree);
        BTreeIterator i(&sorted_tree);

        while (i.hasNext()) {
            Action* a = i.next();
            this->actions.append(a);
            node->add(a);
        }
    }

    this->ready = true;
}

void Tree::runAction() {
    if (this->last != NULL && this->ready)
        last->runAction();
}

Action * Tree::search(QString prefix) {
    if (!this->ready)
        return NULL;
    printf("Searching %s\n", prefix.toStdString().c_str());
    return this->last = this->node->search(prefix);
}
