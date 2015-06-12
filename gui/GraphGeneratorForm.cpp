#include "GraphGeneratorForm.h"
#include "ui_GraphGeneratorForm.h"

#include "MainWindow.h"

GraphGeneratorForm::GraphGeneratorForm(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::GraphGeneratorForm),
    mainWindow(parent)
{
    ui->setupUi(this);
    ui->hypercybeDimentionEdit->setValidator(new QIntValidator(3, 18, this));
    ui->hypercubeWeightEdit->setValidator(
                new QDoubleValidator(0, 1000, 4, this));

    ui->TorusBigRadiusEdit->setValidator(new QIntValidator(3, 1024, this));
    ui->TorusSmallRadiusEdit->setValidator(new QIntValidator(3, 1024, this));
    ui->TorusWeightEdit->setValidator(new QDoubleValidator(0, 1000, 4, this));

    connect(ui->generateHypercubeButton, &QPushButton::clicked,
            this, &GraphGeneratorForm::generateHypercube);
    connect(ui->generateTorusButton, &QPushButton::clicked,
            this, &GraphGeneratorForm::generateTorus);

    connect(ui->hypercybeDimentionEdit, &QLineEdit::textChanged,
            this, &GraphGeneratorForm::hypercubeDataValidate);
    connect(ui->hypercubeWeightEdit, &QLineEdit::textChanged,
            this, &GraphGeneratorForm::hypercubeDataValidate);

    connect(ui->TorusBigRadiusEdit, &QLineEdit::textChanged,
            this, &GraphGeneratorForm::torusDataValidate);
    connect(ui->TorusSmallRadiusEdit, &QLineEdit::textChanged,
            this, &GraphGeneratorForm::torusDataValidate);
    connect(ui->TorusWeightEdit, &QLineEdit::textChanged,
            this, &GraphGeneratorForm::torusDataValidate);
}



void GraphGeneratorForm::hypercubeDataValidate()
{
    bool valid = ui->hypercybeDimentionEdit->hasAcceptableInput();
    valid = valid && ui->hypercubeWeightEdit->hasAcceptableInput();
    ui->generateHypercubeButton->setEnabled(valid);
}



void GraphGeneratorForm::torusDataValidate()
{
    bool valid = ui->TorusBigRadiusEdit->hasAcceptableInput();
    valid = valid && ui->TorusSmallRadiusEdit->hasAcceptableInput();
    valid = valid && ui->TorusWeightEdit->hasAcceptableInput();
    if (valid)
    {
        unsigned small = ui->TorusSmallRadiusEdit->text().toUInt();
        unsigned big = ui->TorusBigRadiusEdit->text().toUInt();
        if (small * big > 262144)
        {
            valid = false;
            mainWindow->showStatusMessage(tr("For torus parameters are to big. "
                                             "Decrease radius."), 5000);
        }
    }
    ui->generateTorusButton->setEnabled(valid);
}



GraphGeneratorForm::~GraphGeneratorForm()
{
    delete ui;
}


void GraphGeneratorForm::generateHypercube()
{
    bool ok;
    uint dimention = ui->hypercybeDimentionEdit->text().toUInt(&ok);
    if (!ok)
    {
        mainWindow->showStatusMessage(tr("Hypercube dimention not a number"));
        return;
    }
    float weight = ui->hypercubeWeightEdit->text().toFloat(&ok);
    if (!ok)
    {
        mainWindow->showStatusMessage(tr("Hypercube edge weight not a number"));
        return;
    }
    ui->generateHypercubeButton->setEnabled(false);
    GraphWorker& graph = mainWindow->getGraph();

    mainWindow->showStatusMessage(tr("Clearing graph..."));
    graph.clear();

    graph.generateHypercube(dimention, weight);
    mainWindow->showStatusMessage(tr("Hypercube created"), 5000);
}

void GraphGeneratorForm::generateTorus()
{
    bool ok;
    uint bigRadius = ui->TorusBigRadiusEdit->text().toUInt(&ok);
    if (!ok)
    {
        mainWindow->showStatusMessage(tr("Torus big radius not a number"));
        return;
    }
    uint smallRadius = ui->TorusSmallRadiusEdit->text().toUInt(&ok);
    if (!ok)
    {
        mainWindow->showStatusMessage(tr("Torus small radius not a number"));
        return;
    }
    float weight = ui->TorusWeightEdit->text().toFloat(&ok);
    if (!ok)
    {
        mainWindow->showStatusMessage(tr("Torus edge weight not a number"));
        return;
    }
    ui->generateTorusButton->setEnabled(false);
    GraphWorker& graph = mainWindow->getGraph();

    mainWindow->showStatusMessage(tr("Clearing graph..."));
    graph.clear();

    graph.generateTorus(bigRadius, smallRadius, weight);
    mainWindow->showStatusMessage(tr("Torus created"), 5000);
}
