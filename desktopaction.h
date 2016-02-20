#ifndef DESKTOPACTION_H
#define DESKTOPACTION_H

#include <QString>
#include <QStack>
#include <QStringList>

#include "action.h"

class DesktopAction: public Action
{
public:
    DesktopAction(QString file,QObject * parent=NULL);
    static QStack<DesktopAction*> LoadDesktopActions();
    static QStringList* GetPaths();
public slots:
    virtual void runAction();
    virtual QString getIcon();
private:
    bool terminal;
    QString cached_icon_path;

    void clear_action();
};



#endif // DESKTOPACTION_H
