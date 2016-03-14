#ifndef BOOKMARKACTION_H
#define BOOKMARKACTION_H

#include <QString>
#include <QUrl>
#include <QStringList>

#include "action.h"
#include "btree.h"

class BookmarkAction: public Action {
public:
    BookmarkAction(QString name, QUrl url, QString icon, QObject* parent=NULL);
    static void LoadBookmarkActions(BTree*);
    static QStringList* GetPaths();
public slots:
    virtual void runAction();
    virtual QString getIcon();
    virtual bool hasCornerIcon();
    virtual QString getCornerIcon();
private:
    QUrl url;
    void download_icon(QUrl url, QString icon, QString destination);
};

#endif // BOOKMARKACTION_H
