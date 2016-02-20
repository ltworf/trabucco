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

    printf("Rescan triggered\n");

    QListIterator<Action*> i(this->actions);
    while (i.hasNext()) {
        delete i.next();
    }
    this->actions.clear();

    delete this->node;
    this->node = new Node();
    QStack<DesktopAction*> actions = DesktopAction::LoadDesktopActions();
    while (!actions.isEmpty()) {
        Action* a = actions.pop();
        this->actions.append(a);
        node->add(a);
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
