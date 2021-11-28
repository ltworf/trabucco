/*
This file is part of Trabucco.

Trabucco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Trabucco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Trabucco.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2016  Salvo "LtWorf" Tomaselli
*/

#include "downloader.h"

#include <QNetworkRequest>
#include <QDir>

Downloader::Downloader(QStringList sources, QString destination, QObject *parent) : QObject(parent) {
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
