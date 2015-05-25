#include <QFileDialog>

#include "SaveShortPathForm.h"
#include "ui_SaveShortPathForm.h"

#include "MainWindow.h"
#include "graph/WriterBase.h"



SaveShortPathForm::SaveShortPathForm(MainWindow* mainWindow, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveShortPathForm),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    ui->startNodeEdit->setValidator(new QIntValidator(0, 1024, this));
    ui->pathLimitEdit->setValidator(new QDoubleValidator(0, 1024, 4, this));

    connect(ui->startNodeEdit, &QLineEdit::textEdited,
            this, &SaveShortPathForm::validateShortPath);
    connect(ui->pathLimitEdit, &QLineEdit::textEdited,
            this, &SaveShortPathForm::validateShortPath);
    connect(ui->printIndentsCheck, &QCheckBox::toggled,
            this, &SaveShortPathForm::validateShortPath);

    connect(ui->saveButton, &QPushButton::clicked,
            this, &SaveShortPathForm::saveShortPath);

    validateShortPath();
}



SaveShortPathForm::~SaveShortPathForm()
{
    delete ui;
}



void SaveShortPathForm::validateShortPath()
{
    ui->saveButton->setEnabled(false);

    Graph* graph = & mainWindow->getGraph();
    bool isEmpty = graph->isGraphEmpty();
    bool ok, isValid = true;
    QString info;

    if (isEmpty)
    {
        info += tr("Graph is empty. Nothing to save.");
        info += '\n';
        isValid = false;
    }

    bool printIndents = ui->printIndentsCheck->isChecked();


    ulong nodeId;
    if (ui->startNodeEdit->hasAcceptableInput())
    {
        nodeId = ui->startNodeEdit->text().toUInt(&ok);
    }
    else
    {
        info += tr("Start node is not number.");
        info += '\n';
        isValid = false;
    }

    if (!ui->pathLimitEdit->hasAcceptableInput())
    {
        info += tr("Path limit is not number.");
        info += '\n';
        isValid = false;
    }


    if (isValid)
    {
        bool nodeExist = graph->findNode(nodeId) != nullptr;
        if (nodeExist)
        {
            bool pathExist = graph->isPathExist(nodeId);
            if (!pathExist)
            {
                info += tr("Path not created. It will be created before save.");
            }
        }
        else
        {
            info += tr("Node %1 not found.").arg(nodeId);
            info += ' ';
            std::deque<size_t> *nodes = graph->findNearNode(nodeId);
            if (nodes)
            {
                info += tr("Nearest:");
                info += ' ';
                for(size_t i = 0; i < nodes->size(); ++i)
                {
                    if (i > 0)
                        info += ", ";
                    info += QString::number(nodes->at(i));
                }
                delete nodes;
            }
            isValid = false;
        }

    }

    ui->infoLabel->setText(info);

    QString example;
    example += FileTypes::typeName(FileTypes::Type::BRACKETS_SHORT_PATH_VALUE);
    example += '\n';

    if (printIndents)
    {
        example += "1(\n    2[5](\n        4[7]\n        5[10]\n    )\n)\n";
    }
    else
    {
        example += "1(2[5](4[7] 5[10]))\n";
    }
    ui->exampleLabel->setText(example);
    ui->saveButton->setEnabled(isValid);
}



void SaveShortPathForm::saveShortPath()
{
    bool ok;
    ulong nodeId = ui->startNodeEdit->text().toUInt(&ok);
    if (!ok)
    {
        ui->infoLabel->setText(tr("Start node is not number."));
        return;
    }

    float pathLimit = ui->pathLimitEdit->text().toFloat(&ok);
    if (!ok)
    {
        ui->infoLabel->setText(tr("Path limit is not number."));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save graph"), "",
        tr("Text files (*.txt)"));

    if (fileName.isEmpty())
    {
        return;
    }
    if (!fileName.endsWith(".txt"))
    {
        fileName += ".txt";
    }

    QByteArray fileNameLatin = fileName.toLatin1();

    unsigned options = WriterBase::Option::NONE;
    if (ui->printIndentsCheck->isChecked())
    {
        options = options | WriterBase::Option::PRINT_INDENTS;
    }
    NodeIdDeque nodes;
    nodes.push_back(nodeId);

    GraphWorker& graph = mainWindow->getGraph();
    bool result = graph.saveShortPaths(fileNameLatin.data(), &nodes, pathLimit,
                                       options);

    if (result)
    {
        int p = fileName.lastIndexOf(QDir::separator());
        QString shortName = fileName.mid(++p);
        mainWindow->showStatusMessage(tr("Short path saved in file %1").
                                    arg(shortName));
    }
    else
    {
        ui->infoLabel->setText(tr("Short path not saved."
                                  "File not accessed to write."));
        mainWindow->showStatusMessage(tr("Short path not saved."), 5000);
    }
}



