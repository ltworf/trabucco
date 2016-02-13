#include "tree.h"

Tree::Tree(QObject *parent) : QObject(parent)
{

    this->node = new Node();


    QStack<DesktopAction*> actions = DesktopAction::LoadDesktopActions();
    while (!actions.isEmpty()) {
        node->add(actions.pop());
    }
}

void Tree::runAction() {
    if (this->last != NULL)
        last->runAction();
}

Action * Tree::search(QString prefix) {
    printf("Searching %s\n", prefix.toStdString().c_str());
    return this->last = this->node->search(prefix);
}
