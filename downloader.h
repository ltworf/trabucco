#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Downloader : public QObject {
    Q_OBJECT
public:
    explicit Downloader(QStringList sources, QString destination, QObject *parent = 0);
private slots:
    void download();
    void finished(QNetworkReply* reply);
private:
    QStringList sources;
    QString destination;
    QFile dest_file;
    QNetworkAccessManager nam;

};

#endif // DOWNLOADER_H
