#ifndef NODE_H
#define NODE_H

#include <QHash>
#include <QString>
#include <QChar>

#include "action.h"

class Node
{
public:
    Node();
    Node(Action* action);
    void add(Action* action);
    Action * search(QString prefix);
private:
    QHash<QChar, Node*> hash;
    Action * action=NULL;

    void add(Action* action, QString name);
    Action * _search(QString prefix);

};

#endif // NODE_H
