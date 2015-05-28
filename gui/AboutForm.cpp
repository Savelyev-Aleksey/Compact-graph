#include <QSettings>

#include "AboutForm.h"
#include "ui_AboutForm.h"

AboutForm::AboutForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutForm)
{
    ui->setupUi(this);

    connect(ui->aboutButton, &QToolButton::clicked,
            qApp, &QApplication::aboutQt);

    connect(ui->languageCombo, static_cast<void(QComboBox::*)(int)>
            (&QComboBox::currentIndexChanged),
            this, &AboutForm::changeLanguage);

    QSettings settings("Compact Graph");
    QString loc = QLocale::system().name();
    QString c = settings.value("locale", loc).toString();


    int pos = ui->languageCombo->findText(c.left(c.indexOf('_')),
                                          Qt::MatchStartsWith);

    ui->languageCombo->setCurrentIndex(pos);
}



AboutForm::~AboutForm()
{
    delete ui;
}


void AboutForm::changeLanguage(int index)
{
    QString s;
    switch (index)
    {
    case 0:
        s = "ru_RU";
        break;
    case 1:
        s = "en_GB";
        break;
    default:
        s = QLocale::system().name();
        break;
    }
    QSettings settings("Compact Graph");
    settings.setValue("locale", s);
}
