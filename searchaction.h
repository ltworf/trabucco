#ifndef SEARCHACTION_H
#define SEARCHACTION_H

#include <QString>
#include <QStringList>
#include <QList>

#include "action.h"
#include "btree.h"

class SearchAction: public Action {
public:
    static QList<SearchAction*> LoadFile(QString file, QObject* parent=NULL);
    static void LoadSearchActions(BTree*);
    static QStringList* GetPaths();
    bool mustShow();
public slots:
    virtual bool isPrefix();
    virtual QString getIcon();
    virtual void runAction(QString);
private:
    SearchAction(QString name, QString query, bool hidden, QObject* parent=NULL);
    QString query;
    QString hidden;
    bool show;
};

#endif // SEARCHACTION_H

