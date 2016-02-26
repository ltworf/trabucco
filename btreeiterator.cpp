#include "btreeiterator.h"

BTreeIterator::BTreeIterator(BTree *tree, QObject* parent) : QObject(parent) {
    can_iterate = tree != NULL ? true : false;

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
            if (item.ptr->getLeft()) {
                stack_item n;
                n.ptr = item.ptr->getLeft();
                n.state = LEFT;
                stack.push(n);
            }
            item.state = SELF;
            stack.push(item);

            break;
        }
        case SELF: {
            if (item.ptr->getRight()!=NULL) {
                stack_item n;
                n.ptr = item.ptr->getRight();
                n.state = LEFT;
                stack.push(n);
            }

            if (!stack.size())
                can_iterate = false;

            return item.ptr->getAction();
            break;
        }
        case RIGHT: {
            break;
        }


        }
    }


}
