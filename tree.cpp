#include "tree.h"

#include <QStringListIterator>

#include "btree.h"
#include "btreeiterator.h"
#include "bookmarkaction.h"

Tree::Tree(QObject *parent) : QObject(parent) {
    rescan();

    this->watcher.addPaths(*DesktopAction::GetPaths());

    connect(
        &this->watcher,
        &QFileSystemWatcher::directoryChanged,
        this,
        &Tree::rescan
    );
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
        DesktopAction::LoadDesktopActions(&sorted_tree);
        BookmarkAction::LoadBookmarkActions(&sorted_tree);
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
