#include "node.h"

#include <QHashIterator>

Node::Node() {
    this->action = NULL;
}

Node::Node(Action* action) {
    this->action = action;
}

void Node::add(Action *action) {
    this->add(action,action->getName().toLower());
}

Node::~Node() {
    QHashIterator<QChar, Node*> i(this->hash);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
}

void Node::add(Action *action, QString name) {
    if (name.length()==0) {
        return;
    }

    Node * n;
    if (this->hash.contains(name[0])) {
        n = hash[name[0]];
    } else {
        n = new Node(action->isPrefix() && name.size()!=1? NULL:action);
        hash.insert(name[0], n);
    }

    if (!this->action && !action->isPrefix())
        this->action = action;

    n->add(action,name.right(name.length() - 1));

}

Action * Node::search(QString prefix) {
    return this->_search(prefix.toLower());
}

Action * Node::_search(QString prefix) {
    Action * result = this->action;
    if (result && result->isPrefix() && prefix.length())
        return result;

    if (prefix.length() && hash.contains(prefix[0])) {
        Node * n = hash.value(prefix[0]);
        result = n->_search(prefix.right(prefix.length() - 1));
    } else if (prefix.length()) {
        result = NULL;
    }
    return result;
}

