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

