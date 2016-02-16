#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QString>

class Action : public QObject
{
    Q_OBJECT
public:
    explicit Action(QObject * parent=NULL);
    explicit Action(QString name, QString action, QString icon, QObject * parent=NULL);

signals:

public slots:
    QString getName();
    QString getAction();
    virtual QString getIcon();
    virtual void runAction();

protected:
    QString name;
    QString action;
    QString icon;
};

#endif // ACTION_H
