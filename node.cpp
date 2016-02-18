#include "node.h"
Node::Node() {
    this->action = NULL;
}

Node::Node(Action* action) {
    this->action = action;
}

void Node::add(Action *action) {
    this->add(action,action->getName().toLower());
}

void Node::add(Action *action, QString name) {
    if (name.length()==0) {
        return;
    }

    Node * n;
    if (this->hash.contains(name[0])) {
        n = hash[name[0]];
    } else {
        n = new Node(action);
        hash.insert(name[0], n);
    }

    if (name.length() > 1) {
        n->add(action,name.right(name.length() - 1));
    } else {
        /*
         * In this way, things that have a name that is
         * a prefix of other things, have a way to be
         * accessed for sure.
         **/
        this->action = action;
    }


}

Action * Node::search(QString prefix) {
    return this->_search(prefix.toLower());
}

Action * Node::_search(QString prefix) {
    Action * result = this->action;

    if (prefix.length() && hash.contains(prefix[0])) {
        Node * n = hash.value(prefix[0]);
        result = n->search(prefix.right(prefix.length() - 1));
    } else if (prefix.length()) {
        result = NULL;
    }
    return result;
}

