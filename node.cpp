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

    if (this->hash.contains(name[0])) {
        Node * n = hash[name[0]];
        n->add(action,name.right(name.length() - 1));
    } else {
        Node * n = new Node(action);
        hash.insert(name[0], n);
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
    }
    return result;
}

