#ifndef SHORTCUTACTIVATOR_H
#define SHORTCUTACTIVATOR_H

#include <QThread>

class ShortcutActivator : public QThread
{
    Q_OBJECT
public:
    explicit ShortcutActivator(QObject *parent = 0);
    void run();
signals:
    void activated();

public slots:
    void end();
private:
    bool terminate = false;
    unsigned char key;
    unsigned int modifier;
};

#endif // SHORTCUTACTIVATOR_H
