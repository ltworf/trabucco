#ifndef DESKTOPACTION_H
#define DESKTOPACTION_H

#include <QString>
#include <QStringList>

#include "action.h"
#include "btree.h"

class DesktopAction: public Action
{
public:
    DesktopAction(QString file,QObject * parent=NULL);
    static void LoadDesktopActions(BTree*);
    static QStringList* GetPaths();
public slots:
    virtual void runAction();
    virtual QString getIcon();
private:
    bool terminal;
    bool cached_icon = false;
    QString cached_icon_path;

    void clear_action();
};



#endif // DESKTOPACTION_H
