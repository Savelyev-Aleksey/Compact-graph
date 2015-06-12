#include <QFileDialog>

#include "SaveForm.h"
#include "ui_SaveForm.h"

#include "MainWindow.h"
#include "SaveGraphForm.h"
#include "SaveShortPathForm.h"
#include "SaveProjectionForm.h"

SaveForm::SaveForm(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::SaveForm),
    mainWindow(parent)
{
    ui->setupUi(this);

    ui->saveStackedWidget->addWidget(new SaveGraphForm(mainWindow, this));
    ui->saveStackedWidget->addWidget(new SaveShortPathForm(mainWindow, this));
    ui->saveStackedWidget->addWidget(new SaveProjectionForm(mainWindow, this));

    // Save menu buttons connect
    connect(ui->menuSaveGraph, &QToolButton::clicked,
            this, &SaveForm::menuShowGraph);
    connect(ui->menuSaveShortPath, &QToolButton::clicked,
            this, &SaveForm::menuShowShortPath);
    connect(ui->menuSaveProjection, &QToolButton::clicked,
            this, &SaveForm::menuShowProjection);
}



SaveForm::~SaveForm()
{
    delete ui;
}



void SaveForm::menuShowGraph()
{
    ui->saveStackedWidget->setCurrentIndex(0);
}



void SaveForm::menuShowShortPath()
{
    ui->saveStackedWidget->setCurrentIndex(1);
}



void SaveForm::menuShowProjection()
{
    ui->saveStackedWidget->setCurrentIndex(2);
}

