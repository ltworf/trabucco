#ifndef DESKTOPACTION_H
#define DESKTOPACTION_H

#include <QString>

#include "action.h"

class DesktopAction: public Action
{
public:
    DesktopAction(QString file,QObject * parent=0);
public slots:
    virtual void runAction();
private:
    bool terminal;
};

#endif // DESKTOPACTION_H
