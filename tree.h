#ifndef TREE_H
#define TREE_H

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>

#include "desktopaction.h"
#include "action.h"
#include "node.h"

class Tree : public QObject {
    Q_OBJECT
public:
    explicit Tree(QObject *parent = 0);

public slots:
    Action * search(QString prefix);
    void runAction();
    void rescan();
private:
    bool ready = false;
    bool bookmarks;
    bool desktop;
    bool searchprovider;
    Action * last = NULL;
    QString last_prefix;
    Node * node = NULL;
    QFileSystemWatcher watcher;
    QList<Action*> actions;
};

#endif // TREE_H
