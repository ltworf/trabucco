#include "settings.h"
#include "ui_settings.h"

#include <QSettings>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    QSettings settings;
    ui->chkBookmarks->setChecked(settings.value("Source/Bookmarks", true).toBool());
    ui->chkDesktop->setChecked(settings.value("Source/Desktop", true).toBool());
}

void Settings::accept() {
    QSettings settings;
    settings.setValue("Source/Bookmarks", this->ui->chkBookmarks->isChecked());
    settings.setValue("Source/Desktop", this->ui->chkDesktop->isChecked());

    QDialog::accept();
}

Settings::~Settings()
{
    delete ui;
}
