#include "tree.h"

#include <QStringListIterator>

Tree::Tree(QObject *parent) : QObject(parent)
{
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
    printf("Rescan triggered %d\n",__LINE__);

    delete this->node;
    this->node = new Node();
    QStack<DesktopAction*> actions = DesktopAction::LoadDesktopActions();
    while (!actions.isEmpty()) {
        node->add(actions.pop());
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
