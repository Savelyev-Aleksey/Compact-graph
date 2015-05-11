#include "AboutForm.h"
#include "ui_AboutForm.h"

AboutForm::AboutForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutForm)
{
    ui->setupUi(this);

    connect(ui->aboutButton, &QToolButton::clicked,
            qApp, &QApplication::aboutQt);
}

AboutForm::~AboutForm()
{
    delete ui;
}
