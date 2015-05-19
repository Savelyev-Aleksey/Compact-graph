#include "GraphInfoForm.h"
#include "ui_GraphInfoForm.h"

#include "MainWindow.h"
#include "graph/Node.h"
#include "graph/Edge.h"



GraphInfoForm::GraphInfoForm(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::GraphInfoForm),
    mainWindow(parent)
{
    ui->setupUi(this);

    connect(ui->findParamsButton, &QPushButton::clicked,
            this, &GraphInfoForm::findParameters);

    printGraphInfo();
    printGraphStatistic();
    printShortPathInfo();
    printShortPathStatistic();
}



GraphInfoForm::~GraphInfoForm()
{
    delete ui;
}



void GraphInfoForm::findParameters()
{
    ui->findParamsButton->setEnabled(false);

    GraphWorker& graph = mainWindow->getGraph();

    graph.generateAllShortPaths();
    graph.updateParameters();

    printShortPathInfo();
    printShortPathStatistic();
}



void GraphInfoForm::printGraphInfo()
{
    ui->FileNameLabel->setText( mainWindow->getOpenFileName() );
    ui->FileNameLabel->setToolTip( mainWindow->getOpenFile() );

    ui->nodeCountLabel->setText(QString::number(Node::getCount()));
    ui->EdgeCountLabel->setText(QString::number(Edge::getCount()));

    const GraphWorker& graph = mainWindow->getGraph();

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



void GraphInfoForm::printShortPathInfo()
{
    const GraphWorker& graph = mainWindow->getGraph();
    size_t value = graph.getShortPathCount();
    ui->pathCountLabel->setText( QString::number(value) );

    value = graph.getDiameter();
    ui->diameterLabel->setText( QString::number(value) );

    value = graph.getRadius();
    ui->radiusLabel->setText( QString::number(value) );
}



void GraphInfoForm::printTableStat(const UlongMap* map, QTableWidget* table,
                                   bool shrink)
{
    const GraphWorker& graph = mainWindow->getGraph();
    size_t count = graph.statisticParamSum(map);
    double percent;

    QTableWidgetItem *item;

    size_t rowCount = map->size();
    table->setRowCount(rowCount);

    unsigned row = 0;
    for (auto it = map->begin(), end = map->end(); it != end;
         ++it, ++row)
    {
        if (shrink && row > 0 && row < rowCount - 1)
            item = new QTableWidgetItem( QString("<= %1").arg(it->first) );
        else
            item = new QTableWidgetItem( QString::number(it->first) );

        table->setItem(row, 0, item);
        item = new QTableWidgetItem( QString::number(it->second) );
        table->setItem(row, 1, item);

        percent = (double) it->second / count * 100;
        item = new QTableWidgetItem( QString::number(percent, 'g', 4) );
        table->setItem(row, 2, item);
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
    bool shrink = map->size() > 10 ? true : false;

    if (map->size() < 3)
        compact = map;
    else
    {
        compact = graph.compactStatistic(map, 10);
        delete map;
    }

    printTableStat(compact, ui->degreeStatTable, shrink);
    delete compact;
}



void GraphInfoForm::printShortPathStatistic()
{
    const GraphWorker& graph = mainWindow->getGraph();

    UlongMap* compact = nullptr;
    UlongMap* map = graph.getEccentriciyStatistic();
    if (!map)
    {
        return;
    }
    bool shrink = map->size() > 10 ? true : false;

    if (map->size() < 3)
        compact = map;
    else
    {
        compact = graph.compactStatistic(map, 10);
        delete map;
    }
    printTableStat(compact, ui->EccentrStatTable, shrink);
    delete compact;
}
