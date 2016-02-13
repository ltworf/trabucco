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
private:
    bool terminal;
};



#endif // DESKTOPACTION_H
