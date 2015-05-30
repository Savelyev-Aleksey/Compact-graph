#include <cstdint>
#include <vector>

#include "graph/Node.h"
#include "graph/Projection.h"

#include "NodeInfoForm.h"
#include "ui_NodeInfoForm.h"

#include "MainWindow.h"

NodeInfoForm::NodeInfoForm(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::NodeInfoForm),
    mainWindow(parent)
{
    ui->setupUi(this);

    ui->nodeNumEdit->setValidator(new QIntValidator(0, INT_MAX, this));

    connect(ui->nodeNumEdit, &QLineEdit::textChanged,
            this, &NodeInfoForm::findNode);
    connect(ui->createProjectionButton, &QPushButton::clicked,
            this, &NodeInfoForm::createProjection);

    const GraphWorker& graph = mainWindow->getGraph();

    if (graph.isGraphEmpty())
    {
        ui->infoLabel->setText(tr("Graph is empty."));
        ui->nodeNumEdit->setEnabled(false);
        return;
    }

    clearInfo();
}



NodeInfoForm::~NodeInfoForm()
{
    delete ui;
}



void NodeInfoForm::clearInfo()
{
    ui->adjacentNodesList->clear();
    ui->createProjectionButton->setEnabled(false);
    ui->levelTable->clearContents();

    ui->eccentricityLabel->clear();
    ui->degreeLabel->clear();
    ui->shortLoopLabel->clear();
}



void NodeInfoForm::findNode()
{
    bool ok = false;

    clearInfo();

    ulong nodeId;
    if (ui->nodeNumEdit->hasAcceptableInput())
        nodeId = ui->nodeNumEdit->text().toULong(&ok);

    QString info;

    if (ok)
    {
        const GraphWorker& graph = mainWindow->getGraph();
        const Node* node = graph.findNode(nodeId, info);
        if (node)
        {
            currentNodeId = nodeId;
            // get adjacent nodes
            const EdgeList& list = *node->getEdges();
            ui->adjacentNodesList->clear();

            for(auto const &it : list)
            {
                ui->adjacentNodesList->addItem(QString::number(it.first));
            }
            ui->degreeLabel->setText(QString::number(list.size()));
            projectionInfo();
        }
    }
    else
    {
        info = tr("Start node is not number.");
    }
    ui->infoLabel->setText(info);
}



void NodeInfoForm::projectionInfo()
{
    const GraphWorker& graph = mainWindow->getGraph();
    const Projection* pr = graph.getProjection(currentNodeId);
    if (!pr)
    {
        ui->createProjectionButton->setEnabled(true);
        return;
    }
    unsigned eccentr = pr->getEccentricity();
    ui->eccentricityLabel->setText(QString::number(eccentr));

    unsigned shortLoop = pr->getShortestLoop();
    QString shortLoopStr("\u221E (inf)");
    if (shortLoop != UINT32_MAX)
    {
        shortLoopStr = QString::number(shortLoop);
    }

    ui->shortLoopLabel->setText(shortLoopStr);

    uint_vec* list = pr->getProjectionNodeStat();
    QTableWidget* t = ui->levelTable;
    unsigned size = list->size();
    t->clearContents();
    t->setRowCount(size >> 1);
    for (unsigned i = 0; i < size; i += 2)
    {
        unsigned orig = (*list)[i];
        unsigned replica = (*list)[i + 1];
        unsigned pos = i >> 1;
        t->setItem(pos, 0, new QTableWidgetItem(QString::number(pos)) );
        t->setItem(pos, 1, new QTableWidgetItem(QString::number(orig)) );
        t->setItem(pos, 2, new QTableWidgetItem(QString::number(replica)) );
        t->setItem(pos, 3, new QTableWidgetItem(QString::number(orig+replica)));
    }
    t->resizeColumnsToContents();
    delete list;
}



void NodeInfoForm::createProjection()
{
    ui->createProjectionButton->setEnabled(false);
    GraphWorker& graph = mainWindow->getGraph();
    graph.createProjection(currentNodeId);

    if (!graph.isInterrupted())
    {
        projectionInfo();
    }
}
