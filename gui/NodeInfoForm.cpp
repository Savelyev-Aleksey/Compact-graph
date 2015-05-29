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
    size_vec* list = pr->getProjectionNodeStat();
    QTableWidget* t = ui->levelTable;
    for (size_t i = 0, end = list->size(); i < end; i += 2)
    {
        size_t orig = (*list)[i];
        size_t replica = (*list)[i + 1];
        t->setItem(0, 0, new QTableWidgetItem(QString::number(i >> 1)) );
        t->setItem(0, 1, new QTableWidgetItem(QString::number(orig)) );
        t->setItem(0, 2, new QTableWidgetItem(QString::number(replica)) );
    }
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
