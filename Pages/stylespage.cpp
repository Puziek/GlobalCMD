#include "stylespage.h"
#include "ui_stylespage.h"
#include <QDebug>

StylesPage::StylesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StylesPage)
{
    ui->setupUi(this);

    ui->cb_styles->addItem("Default");
    ui->cb_styles->addItems(QDir(":Styles/").entryList());

     ui->cb_styles->setCurrentIndex(ui->cb_styles->
                                    findText(QDir(SettingsManager::getSetting("Styles", "Current style", "").toString()).dirName()));

    connect(ui->cb_styles, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(styleChanged(QString)));
}

StylesPage::~StylesPage()
{
    delete ui;
}
