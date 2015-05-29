#include "StatisticMenuForm.h"
#include "ui_StatisticMenuForm.h"

#include "MainWindow.h"

#include "GraphInfoForm.h"
#include "NodeInfoForm.h"



StatisticMenuForm::StatisticMenuForm(MainWindow* parent) :
    QWidget(parent),
    ui(new Ui::StatisticMenuForm)
{
    ui->setupUi(this);
    ui->statisticStackedWidget->addWidget(new GraphInfoForm(parent));
    ui->statisticStackedWidget->addWidget(new NodeInfoForm(parent));

    ui->statisticStackedWidget->setCurrentIndex(0);

    connect(ui->menuGraph, &QToolButton::clicked,
            this, &StatisticMenuForm::menuShowGraph);
    connect(ui->menuNode, &QToolButton::clicked,
            this, &StatisticMenuForm::menuShowNode);
}



StatisticMenuForm::~StatisticMenuForm()
{
    delete ui;
}



void StatisticMenuForm::menuShowGraph()
{
    ui->statisticStackedWidget->setCurrentIndex(0);
}



void StatisticMenuForm::menuShowNode()
{
    ui->statisticStackedWidget->setCurrentIndex(1);
}
