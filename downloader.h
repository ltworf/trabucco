#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QStringList>
#include <QString>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QStringList sources, QString destination, QObject *parent = 0);

signals:

public slots:
};

#endif // DOWNLOADER_H
