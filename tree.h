#ifndef TREE_H
#define TREE_H

#include <QObject>
#include <QString>

#include "desktopaction.h"
#include "action.h"
#include "node.h"

class Tree : public QObject
{
    Q_OBJECT
public:
    explicit Tree(QObject *parent = 0);

signals:

public slots:
    Action * search(QString prefix);
    void runAction();

private:
    Action * last = NULL;
    Node * node;
};

#endif // TREE_H
