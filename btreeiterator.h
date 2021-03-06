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

#ifndef BTREEITERATOR_H
#define BTREEITERATOR_H

#include <QObject>
#include <QStack>
#include "btree.h"

typedef enum {
    LEFT,
    SELF,
} iter_state;

typedef struct {
    BTree* ptr;
    iter_state state;
} stack_item;

class BTreeIterator : public QObject {
    Q_OBJECT
public:
    BTreeIterator(BTree*, QObject* parent=NULL);
    Action* next();
    bool hasNext();
private:
    QStack<stack_item> stack;
    bool can_iterate;
};

#endif // BTREEITERATOR_H
