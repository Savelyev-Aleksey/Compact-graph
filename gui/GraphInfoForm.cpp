#include <cstdint>

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

    connect(ui->graphInfoButton, &QPushButton::clicked,
            this, &GraphInfoForm::readProjectionsInfo);
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

    projectionsInfoStatus();
    updateButtonsStatus();
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

    unsigned size = graph.nodeCount();
    bool isProj = false;
    bool isShort = false;
    bool getInfo = false;
    if (size)
    {
        unsigned proj = graph.projectionsCount();
        unsigned shortp = graph.shortPathsCount();
        isProj = proj ? proj != size : true;
        isShort = shortp ? shortp != size : true;

        getInfo = true;

        if (graph.isProjectionsMemoryUsed())
        {
            getInfo = false;
        }
        else if (graph.getProjectionsStatus() != GraphWorker::Status::ALL)
        {
            getInfo = true;
            isProj = true;
        }
    }
    ui->createProjectionsButton->setEnabled(isProj);
    ui->createShorPathButton->setEnabled(isShort);

    ui->graphInfoButton->setEnabled(getInfo);
}



void GraphInfoForm::createShortPaths()
{
    ui->createShorPathButton->setEnabled(false);

    GraphWorker& graph = mainWindow->getGraph();

    graph.createAllShortPaths();
    graph.updateParametersByShortPaths();

    printGraphParameters();
    eccentrType = 0;
    printEccentriciyStatistic();
}




void GraphInfoForm::createProjections()
{
    ui->createProjectionsButton->setEnabled(false);

    GraphWorker& graph = mainWindow->getGraph();
    if (!graph.isProjectionsMemoryUsed() &&
        !graph.getFileName().length())
    {
        ui->infoLabel->setText(
            tr("Graph not saved yet.") + ' '
            + tr("Before save projections save graph.") + ' '
            + tr("Graph is large. Each projection will be saved in "
                 "own file near graph file.") );

        ui->createShorPathButton->setEnabled(false);
        return;
    }

    graph.createAllProjections();
    if (graph.isInterrupted())
        ui->createProjectionsButton->setEnabled(true);

    graph.updateParametersByProjections();

    printGraphParameters();
    eccentrType = 1;
    printEccentriciyStatistic();
}



void GraphInfoForm::printGraphParameters()
{
    const GraphWorker& graph = mainWindow->getGraph();

    ui->FileNameLabel->setText( mainWindow->getOpenFileName() );
    ui->FileNameLabel->setToolTip( mainWindow->getOpenFile() );

    ui->nodeCountLabel->setText( QString::number( graph.nodeCount() ) );
    ui->EdgeCountLabel->setText( QString::number( graph.edgeCount() ) );


    unsigned value = graph.shortPathsCount();
    ui->shortPathsLabel->setText( QString::number(value) );

    value = graph.projectionsCount();
    ui->projectionsLabel->setText( QString::number(value) );

    value = graph.getDiameter();
    if (value)
        ui->diameterLabel->setText( QString::number(value) );

    value = graph.getRadius();
    if (value)
        ui->radiusLabel->setText( QString::number(value) );

    unsigned proj = graph.projectionsCount();
    unsigned shortp = graph.shortPathsCount();

    if (proj)
    {
        eccentrType = 1;
        printEccentriciyStatistic();
        if (graph.isProjectionsMemoryUsed() ||
            graph.getProjectionsStatus() == FileProjectionsFacade::Status::ALL)
        {
            QString g("\u221E (inf)");
            unsigned girth = graph.getGraphGirth();
            if (girth == 0)
            {
                g = '-';
            }
            else if (girth != UINT32_MAX)
            {
                g = QString::number(girth);
            }
            ui->graphGirthLabel->setText(g);
        }
        projectionsInfoStatus();
    }
    else if (shortp)
    {
        eccentrType = 0;
        printEccentriciyStatistic();
    }
}



void GraphInfoForm::printTableStat(const UintMap* map, QTableWidget* table,
                                   bool shrink)
{
    const GraphWorker& graph = mainWindow->getGraph();
    unsigned count = graph.statisticParamSum(map);
    double percent;

    QTableWidgetItem *item;

    unsigned rowCount = map->size();
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

    UintMap* compact = nullptr;
    UintMap* map = graph.getNodeDegreeStatistic();
    if (!map)
    {
        return;
    }
    bool detail = ui->nodesDetailCheck->isChecked();
    unsigned short limit = 6;
    bool shrink = !detail && map->size() > limit ? true : false;
    ui->nodesDetailCheck->setEnabled(shrink);

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

    UintMap* compact = nullptr;
    UintMap* map = nullptr;
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
    ui->eccentrDetailCheck->setEnabled(shrink);

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



/**
 * @brief GraphInfoForm::readProjectionsInfo for saved graph reading file
 * projections for get projections parameters and update graph parameters
 * (diameter, radius, girth).
 */
void GraphInfoForm::readProjectionsInfo()
{
    GraphWorker& graph = mainWindow->getGraph();
    if (!graph.isProjectionsMemoryUsed() &&
        !graph.getFileName().length())
    {
        ui->infoLabel->setText(tr("Graph not saved yet.") + ' ' +
            tr("Before get graph info save graph in file and "
               "create projections."));
        return;
    }

    graph.readProjectionsInfo();

    if (graph.getLastError() == GraphWorker::Error::TYPE)
    {
        ui->infoLabel->setText(tr("Some projection files have unknown format.")
                               + ' ' +
                               tr("Not all projections files are readed."));
    }
    else if (graph.getProjectionsStatus() == GraphWorker::Status::PARTIAL)
    {
        ui->infoLabel->setText(tr("Some projections files are not found."));
    }
    else
    {
        ui->infoLabel->setText("");
    }

    graph.updateParametersByProjections();

    printGraphParameters();
    updateButtonsStatus();
}



void GraphInfoForm::projectionsInfoStatus()
{
    GraphWorker& graph = mainWindow->getGraph();

    if (graph.isProjectionsMemoryUsed())
        return;

    QString info;
    switch (graph.getProjectionsStatus())
    {
    case GraphWorker::Status::NONE:
        info = tr("Projections files are not readed yet.");
        break;
    case GraphWorker::Status::EMPTY:
        info = tr("Projections files are not created yet.");
        break;
    case GraphWorker::Status::PARTIAL:
        info = tr("Some projections files are not found.")
                + ' ' +
                tr("Not all projections files are readed.");
        break;
    case GraphWorker::Status::ALL: // no break
    default:
        info = "";
        break;
    }
    ui->infoLabel->setText(info);
}
