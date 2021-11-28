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

#include "btreeiterator.h"

BTreeIterator::BTreeIterator(BTree *tree, QObject* parent) : QObject(parent) {
    can_iterate = tree != NULL;

    stack_item root;
    root.ptr = tree;
    root.state = LEFT;
    stack.push(root);
}

bool BTreeIterator::hasNext() {
    return can_iterate;
}

Action* BTreeIterator::next() {

    while (true) {
        stack_item item = stack.pop();
        switch (item.state) {
        case LEFT: {
            item.state = SELF;
            stack.push(item);

            if (item.ptr->getLeft()) {
                stack_item n;
                n.ptr = item.ptr->getLeft();
                n.state = LEFT;
                stack.push(n);
            }

            break;
        }
        case SELF: {
            if (item.ptr->getRight()!=NULL) {
                stack_item n;
                n.ptr = item.ptr->getRight();
                n.state = LEFT;
                stack.push(n);
            } else if (!stack.size())
                can_iterate = false;

            return item.ptr->getAction();
            break;
        }
        }
    }


}
