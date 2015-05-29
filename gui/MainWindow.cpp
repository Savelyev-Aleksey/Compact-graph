#include <QPixmap>
#include <QIcon>
#include <QDir>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SaveForm.h"
#include "OpenFileForm.h"
#include "GraphGeneratorForm.h"
#include "StatisticMenuForm.h"
#include "AboutForm.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    graph(this)
{
    ui->setupUi(this);

    ui->mainHorizontalLayout->addWidget(new OpenFileForm(this));

    connect(ui->menuOpenFileButton,  &QToolButton::clicked,
            this, &MainWindow::showOpen);
    connect(ui->menuSaveFileButton,  &QToolButton::clicked,
            this, &MainWindow::showSaveToFile);
    connect(ui->menuGenerateButton,  &QToolButton::clicked,
            this, &MainWindow::showGenerate);
    connect(ui->menuStatisticButton, &QToolButton::clicked,
            this, &MainWindow::showStatistic);
    connect(ui->menuAboutButton, &QToolButton::clicked,
            this, &MainWindow::showAbout);
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::setOpenFileName(const QString &fileName)
{
    openFile = fileName;
    int p = fileName.lastIndexOf(QDir::separator());
    openFileName = fileName.mid(++p);
}



const QString& MainWindow::getOpenFile() const
{
    return openFile;
}



const QString& MainWindow::getOpenFileName() const
{
    return openFileName;
}



void MainWindow::showOpen()
{
    if (!ui->mainHorizontalLayout->isEmpty())
    {
        QWidget* w = ui->mainHorizontalLayout->itemAt(0)->widget();
        OpenFileForm* open = dynamic_cast<OpenFileForm *>(w);
        // If this is same object - do nothing
        if (open)
        {
            return;
        }
        ui->mainHorizontalLayout->removeWidget(w);
        delete w;
    }
    ui->mainHorizontalLayout->addWidget(new OpenFileForm(this));
}



void MainWindow::showSaveToFile()
{
    if (!ui->mainHorizontalLayout->isEmpty())
    {
        QWidget* w = ui->mainHorizontalLayout->itemAt(0)->widget();
        SaveForm* open = dynamic_cast<SaveForm *>(w);
        // If this is same object - do nothing
        if (open)
        {
            return;
        }
        ui->mainHorizontalLayout->removeWidget(w);
        delete w;
    }
    ui->mainHorizontalLayout->addWidget(new SaveForm(this));
}



void MainWindow::showGenerate()
{
    if (!ui->mainHorizontalLayout->isEmpty())
    {
        QWidget* w = ui->mainHorizontalLayout->itemAt(0)->widget();
        GraphGeneratorForm* open = dynamic_cast<GraphGeneratorForm *>(w);
        // If this is same object - do nothing
        if (open)
        {
            return;
        }
        ui->mainHorizontalLayout->removeWidget(w);
        delete w;
    }
    ui->mainHorizontalLayout->addWidget(new GraphGeneratorForm(this));
}



void MainWindow::showStatistic()
{
    if (!ui->mainHorizontalLayout->isEmpty())
    {
        QWidget* w = ui->mainHorizontalLayout->itemAt(0)->widget();
        StatisticMenuForm* open = dynamic_cast<StatisticMenuForm *>(w);
        // If this is same object - do nothing
        if (open)
        {
            return;
        }
        ui->mainHorizontalLayout->removeWidget(w);
        delete w;
    }
    ui->mainHorizontalLayout->addWidget(new StatisticMenuForm(this));
}


void MainWindow::showAbout()
{
    if (!ui->mainHorizontalLayout->isEmpty())
    {
        QWidget* w = ui->mainHorizontalLayout->itemAt(0)->widget();
        AboutForm* open = dynamic_cast<AboutForm *>(w);
        // If this is same object - do nothing
        if (open)
        {
            return;
        }
        ui->mainHorizontalLayout->removeWidget(w);
        delete w;
    }
    ui->mainHorizontalLayout->addWidget(new AboutForm(this));
}



GraphWorker& MainWindow::getGraph()
{
    return graph;
}



void MainWindow::showStatusMessage(const QString &status, int timeout)
{
    ui->statusBar->showMessage(status,  timeout);
}

