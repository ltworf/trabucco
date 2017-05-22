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

#include "settings.h"
#include "ui_settings.h"

#include <QSettings>
#include <QComboBox>
#include <X11/X.h>


static void selectData(QComboBox* box, QVariant data) {
    int id = box->findData(data);
    if (id == -1)
        id = 0;
    box->setCurrentIndex(id);
}

static void addModifiers(QComboBox* box, unsigned int modifier) {
    box->addItem("Alt", Mod1Mask);
    box->addItem("Ctrl", ControlMask);

    selectData(box, modifier);
}

static void addKeys(QComboBox* box, QString key) {

    /*
     * When adding new keys, make sure that the "data" is
     * supported by XStringToKeysym
     */
    for (char i = 'A'; i <= 'Z'; i++)
        box->addItem(QString(i), QString(i));
    box->addItem("Space","space");

    selectData(box, key);
}

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings) {
    ui->setupUi(this);

    QSettings settings;
    ui->chkBookmarks->setChecked(settings.value("Source/Bookmarks", true).toBool());
    ui->chkDesktop->setChecked(settings.value("Source/Desktop", true).toBool());
    ui->chkSearch->setChecked(settings.value("Source/SearchProvider", true).toBool());
    ui->spnDuration->setValue(settings.value("duration", 250).toInt());
    ui->spnOpacity->setValue(settings.value("opacity", 0.8).toDouble());

    unsigned int modifier = settings.value("Shortcut/modifier", Mod1Mask).toUInt();
    QString key = settings.value("Shortcut/keycode", "space").toString();
    addModifiers(this->ui->cmbModifier, modifier);
    addKeys(this->ui->cmbKey, key);
}

void Settings::accept() {
    QSettings settings;
    settings.setValue("Source/Bookmarks", this->ui->chkBookmarks->isChecked());
    settings.setValue("Source/Desktop", this->ui->chkDesktop->isChecked());
    settings.setValue("Source/SearchProvider", this->ui->chkSearch->isChecked());

    settings.setValue("Shortcut/modifier", this->ui->cmbModifier->currentData());
    settings.setValue("Shortcut/keycode", this->ui->cmbKey->currentData());

    settings.setValue("duration", this->ui->spnDuration->value());
    settings.setValue("opacity", this->ui->spnOpacity->value());

    QDialog::accept();
}

Settings::~Settings() {
    delete ui;
}

void Settings::grab_error() {
    this->ui->tabWidget->setCurrentIndex(1);
    this->ui->lblGrabError->setText("Shortcut already in use. Select a different one");
    this->show();
}
