#include "btree.h"

BTree::BTree(QObject *parent) : QObject(parent) {}

Action* BTree::getAction() {
    return action;
}

void BTree::add(Action* action) {
    if (!this->action) {
        this->action = action;
        return;
    }

    bool go_left = action->getName() < this->action->getName();

    if (go_left && !left)
        left = new BTree(this);
    else if (!go_left && !right)
        right = new BTree(this);


    if (go_left)
        left->add(action);
    else
        right->add(action);
}
