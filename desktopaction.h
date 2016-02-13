#ifndef DESKTOPACTION_H
#define DESKTOPACTION_H

#include <QString>
#include <QStack>

#include "action.h"

class DesktopAction: public Action
{
public:
    DesktopAction(QString file,QObject * parent=0);
    static QStack<DesktopAction*> LoadDesktopActions();
public slots:
    virtual void runAction();
    virtual QString getIcon();
private:
    bool terminal;
    QString cached_icon_path;
};



#endif // DESKTOPACTION_H
