#ifndef TREE_H
#define TREE_H

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>

#include "desktopaction.h"
#include "action.h"
#include "node.h"

class Tree : public QObject
{
    Q_OBJECT
public:
    explicit Tree(QObject *parent = 0);

public slots:
    Action * search(QString prefix);
    void runAction();
    void rescan();
private:
    bool ready = false;
    Action * last = NULL;
    Node * node = NULL;
    QFileSystemWatcher watcher;
};

#endif // TREE_H
