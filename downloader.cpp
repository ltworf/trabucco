#include "downloader.h"

#include <QNetworkRequest>
#include <QDir>

Downloader::Downloader(QStringList sources, QString destination, QObject *parent) : QObject(parent)
{
    this->sources = sources;
    this->destination = destination;
    //this->dest_file = QFile(destination);

    connect(
        &this->nam,
        &QNetworkAccessManager::finished,
        this,
        &Downloader::finished
    );

    this->download();

    //TODO place a lock on the destination file
}

void Downloader::finished(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        this->download();
        return;
    }
    QString tmp = destination + "_temp";

    QFile dest(tmp);
    dest.open(QIODevice::WriteOnly);

    dest.write(reply->readAll());
    dest.close();

    QDir d;
    d.remove(destination);
    d.rename(tmp, destination);

}

void Downloader::download() {
    if (this->sources.isEmpty())
        return;

    QUrl url(this->sources.at(0));
    this->sources.removeFirst();

    QNetworkRequest request(url);
    this->nam.get(request);
}
