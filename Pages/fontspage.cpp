#include "fontspage.h"
#include "ui_fontspage.h"


FontsPage::FontsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontsPage)
{
    ui->setupUi(this);

    connect(ui->pb_changeMainFont, &QPushButton::clicked,
            dynamic_cast<SettingsWindow*> (parent), &SettingsWindow::showMainFontDialog);
    connect(ui->pb_changeListFont, &QPushButton::clicked,
            dynamic_cast<SettingsWindow*> (parent), &SettingsWindow::showListFontDialog);
}

FontsPage::~FontsPage()
{
    delete ui;
}

void FontsPage::setMainFont(const QFont& font)
{
    ui->l_mainFontName->setText(font.family() + ", " +  QString::number(font.pointSize())
                                + (font.bold() ? ", b" : ""));
    ui->l_testMainFont->setFont(font);
}

void FontsPage::setListFont(const QFont& font)
{
    ui->l_listFontName->setText(font.family() + ", " +  QString::number(font.pointSize())
                                + (font.bold() ? ", b" : ""));
    ui->l_testListFont->setFont(font);
}
