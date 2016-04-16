#include "btree.h"

BTree::BTree(QObject *parent) : QObject(parent) {}

Action* BTree::getAction() {
    return action;
}

BTree* BTree::getLeft() {
    return left;
}

BTree* BTree::getRight() {
    return right;
}

bool BTree::add(Action* action) {
    if (!this->action) {
        this->action = action;
        return true;
    } else if (this->action->getName() == action->getName()) {
        //Avoid duplicates
        return false;
    }

    bool go_left = action->getName().compare(this->action->getName(), Qt::CaseInsensitive) < 0;

    if (go_left && !left)
        left = new BTree(this);
    else if (!go_left && !right)
        right = new BTree(this);

    if (go_left)
        return left->add(action);
    else
        return right->add(action);
}
