#ifndef SEARCHACTION_H
#define SEARCHACTION_H

#include <QString>
#include <QStringList>

#include "action.h"
#include "btree.h"

class SearchAction: public Action {
public:
    SearchAction(QString file,QObject * parent=NULL);
    static void LoadSearchActions(BTree*);
    static QStringList* GetPaths();
    bool mustShow();
public slots:
    virtual bool isPrefix();
    virtual QString getIcon();
private:

};

#endif // SEARCHACTION_H

