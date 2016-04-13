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
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QSettings settings;
    ui->chkBookmarks->setChecked(settings.value("Source/Bookmarks", true).toBool());
    ui->chkDesktop->setChecked(settings.value("Source/Desktop", true).toBool());
    ui->chkSearch->setChecked(settings.value("Source/SearchProvider", true).toBool());
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

    QDialog::accept();
}

Settings::~Settings()
{
    delete ui;
}
