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
