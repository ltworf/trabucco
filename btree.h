#ifndef BTREE_H
#define BTREE_H

#include <QObject>

#include "action.h"

class BTree : public QObject {
    Q_OBJECT
public:
    explicit BTree(QObject * parent=NULL);
public slots:
    void add(Action* action);
private:
    Action* action = NULL;
    BTree* left = NULL;
    BTree* right = NULL;
};

#endif // BTREE_H
