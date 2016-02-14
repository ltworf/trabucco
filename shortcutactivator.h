#ifndef SHORTCUTACTIVATOR_H
#define SHORTCUTACTIVATOR_H

#include <QThread>

class ShortcutActivator : public QThread
{
    Q_OBJECT
public:
    void run();
signals:
    void activated();

public slots:
    void end();
private:
    bool terminate = false;
};

#endif // SHORTCUTACTIVATOR_H
