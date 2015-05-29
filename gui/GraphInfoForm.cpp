#include "GraphInfoForm.h"
#include "ui_GraphInfoForm.h"

#include "MainWindow.h"
#include "graph/Node.h"
#include "graph/Edge.h"



GraphInfoForm::GraphInfoForm(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::GraphInfoForm),
    mainWindow(parent),
    eccentrType(0)
{
    ui->setupUi(this);

    connect(ui->createShorPathButton, &QPushButton::clicked,
            this, &GraphInfoForm::createShortPaths);
    connect(ui->createProjectionsButton, &QPushButton::clicked,
            this, &GraphInfoForm::createProjections);

    connect(ui->nodesDetailCheck, &QCheckBox::toggled,
            this, &GraphInfoForm::printGraphStatistic);
    connect(ui->eccentrDetailCheck, &QCheckBox::toggled,
            this, &GraphInfoForm::printEccentriciyStatistic);

    ui->degreeStatTable->resizeColumnsToContents();
    ui->EccentrStatTable->resizeColumnsToContents();

    updateButtonsStatus();
    printGraphInfo();
    printGraphStatistic();
    printGraphParameters();
}



GraphInfoForm::~GraphInfoForm()
{
    delete ui;
}



void GraphInfoForm::updateButtonsStatus()
{
    GraphWorker& graph = mainWindow->getGraph();

    size_t size = graph.nodeCount();
    bool isProj = false;
    bool isShort = false;
    if (size)
    {
        size_t proj = graph.projectionsCount();
        size_t shortp = graph.shortPathsCount();
        isProj = proj ? proj != size : true;
        isShort = shortp ? shortp != size : true;
    }
    ui->createProjectionsButton->setEnabled(isProj);
    ui->createShorPathButton->setEnabled(isShort);
}



void GraphInfoForm::createShortPaths()
{
    ui->createShorPathButton->setEnabled(false);

    GraphWorker& graph = mainWindow->getGraph();

    graph.generateAllShortPaths();
    graph.updateParametersByShortPaths();

    printGraphParameters();
    eccentrType = 0;
    printEccentriciyStatistic();
}




void GraphInfoForm::createProjections()
{
    ui->createProjectionsButton->setEnabled(false);

    GraphWorker& graph = mainWindow->getGraph();

    graph.createAllProjections();
    graph.updateParametersByProjections();

    printGraphParameters();
    eccentrType = 1;
    printEccentriciyStatistic();
}



void GraphInfoForm::printGraphInfo()
{
    ui->FileNameLabel->setText( mainWindow->getOpenFileName() );
    ui->FileNameLabel->setToolTip( mainWindow->getOpenFile() );

    const GraphWorker& graph = mainWindow->getGraph();

    ui->nodeCountLabel->setText( QString::number( graph.nodeCount() ) );
    ui->EdgeCountLabel->setText( QString::number( graph.edgeCount() ) );



    const InfoDeque *info = graph.getAllInfo();
    QString infoStr;
    for (size_t i = 0, end = info->size(); i < end; ++i)
    {
        infoStr += info->at(i).first;
        infoStr += " = ";
        infoStr += info->at(i).second;
        infoStr += '\n';
    }
    ui->paramsLabel->setText(infoStr);
}



void GraphInfoForm::printGraphParameters()
{
    const GraphWorker& graph = mainWindow->getGraph();

    size_t value = graph.shortPathsCount();
    ui->shortPathsLabel->setText( QString::number(value) );

    value = graph.projectionsCount();
    ui->projectionsLabel->setText( QString::number(value) );

    value = graph.getDiameter();
    ui->diameterLabel->setText( QString::number(value) );

    value = graph.getRadius();
    ui->radiusLabel->setText( QString::number(value) );

    size_t proj = graph.projectionsCount();
    size_t shortp = graph.shortPathsCount();

    if (proj)
    {
        eccentrType = 1;
        printEccentriciyStatistic();
        QString g("\u221E (inf)");
        size_t girth = graph.getGraphGirth();
        if (girth != SIZE_MAX)
        {
            g = QString::number(girth);
        }
        ui->graphGirthLabel->setText(g);
    }
    else if (shortp)
    {
        eccentrType = 0;
        printEccentriciyStatistic();
    }
}



void GraphInfoForm::printTableStat(const UlongMap* map, QTableWidget* table,
                                   bool shrink)
{
    const GraphWorker& graph = mainWindow->getGraph();
    size_t count = graph.statisticParamSum(map);
    double percent;

    QTableWidgetItem *item;

    size_t rowCount = map->size();
    table->clearContents();
    table->setRowCount(rowCount);

    unsigned row = 0;
    for (auto it = map->begin(), end = map->end(); it != end; ++it, ++row)
    {
        if (shrink && row > 0 && row < rowCount - 1)
        {
            item = new QTableWidgetItem("<=");
            table->setItem(row, 0, item);
        }




        item = new QTableWidgetItem( QString::number(it->first) );
        table->setItem(row, 1, item);
        item = new QTableWidgetItem( QString::number(it->second) );
        table->setItem(row, 2, item);

        percent = (double) it->second / count * 100;
        item = new QTableWidgetItem( QString::number(percent, 'g', 4) );
        table->setItem(row, 3, item);
    }
    table->resizeColumnsToContents();
}



void GraphInfoForm::printGraphStatistic()
{
    const GraphWorker& graph = mainWindow->getGraph();

    UlongMap* compact = nullptr;
    UlongMap* map = graph.getNodeDegreeStatistic();
    if (!map)
    {
        return;
    }
    bool detail = ui->nodesDetailCheck->isChecked();
    unsigned short limit = 6;
    bool shrink = !detail && map->size() > limit ? true : false;

    if (map->size() < 3 || detail)
        compact = map;
    else
    {
        compact = graph.compactStatistic(map, limit);
        delete map;
    }
    ui->minDegreeLabel->setText( QString::number(compact->begin()->first) );
    ui->maxDegreeLabel->setText( QString::number(compact->rbegin()->first) );

    printTableStat(compact, ui->degreeStatTable, shrink);
    delete compact;
}



void GraphInfoForm::printEccentriciyStatistic()
{
    const GraphWorker& graph = mainWindow->getGraph();

    UlongMap* compact = nullptr;
    UlongMap* map = nullptr;
    if (eccentrType == 0)
        map = graph.getEccentriciyStatisticByShortPaths();
    else if (eccentrType == 1)
        map = graph.getEccentriciyStatisticByProjections();

    if (!map)
    {
        return;
    }
    bool detail = ui->eccentrDetailCheck->isChecked();
    unsigned short limit = 6;
    bool shrink = !detail && map->size() > limit ? true : false;

    if (map->size() < 3 || detail)
        compact = map;
    else
    {
        compact = graph.compactStatistic(map, limit);
        delete map;
    }
    printTableStat(compact, ui->EccentrStatTable, shrink);
    delete compact;
}
