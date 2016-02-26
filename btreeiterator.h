#ifndef BTREEITERATOR_H
#define BTREEITERATOR_H

#include <QObject>
#include <QStack>
#include "btree.h"

typedef enum {
    LEFT,
    SELF,
    RIGHT,
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
