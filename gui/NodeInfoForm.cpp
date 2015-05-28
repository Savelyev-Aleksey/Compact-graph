#include "NodeInfoForm.h"
#include "ui_NodeInfoForm.h"

NodeInfoForm::NodeInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeInfoForm)
{
    ui->setupUi(this);
}

NodeInfoForm::~NodeInfoForm()
{
    delete ui;
}
