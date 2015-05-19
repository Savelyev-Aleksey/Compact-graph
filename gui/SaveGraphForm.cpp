#include <QFileDialog>

#include "SaveGraphForm.h"
#include "ui_SaveGraphForm.h"

#include "MainWindow.h"
#include "graph/Node.h"



SaveGraphForm::SaveGraphForm(MainWindow* mainWindow, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SaveGraphForm),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    prepareGraphWidgets();
}



SaveGraphForm::~SaveGraphForm()
{
    delete ui;
}



void SaveGraphForm::prepareGraphWidgets()
{
    // File type radio connect
    connect(ui->nodeNodeValueRadio, &QRadioButton::clicked,
            this, &SaveGraphForm::graphTypeChanged);
    connect(ui->bracketsFlatRadio, &QRadioButton::clicked,
            this, &SaveGraphForm::graphTypeChanged);
    connect(ui->bracketsNestedRadio, &QRadioButton::clicked,
            this, &SaveGraphForm::graphTypeChanged);


    // Set validators
    ui->pathLimitLineEdit->setValidator(new QIntValidator(1, 100000, this));
    ui->startNodeLineEdit->setValidator(new QIntValidator(0, 100000, this));


    // each options change call validate slot
    connect(ui->printIndentsCheck, &QCheckBox::clicked,
            this, &SaveGraphForm::validateGraphSave);
    connect(ui->printInfoCheck,  &QCheckBox::clicked,
            this, &SaveGraphForm::validateGraphSave);
    connect(ui->printValueCheck, &QCheckBox::clicked,
            this, &SaveGraphForm::validateGraphSave);
    connect(ui->startNodeLineEdit, &QLineEdit::textEdited,
            this, &SaveGraphForm::validateGraphSave);
    connect(ui->pathLimitLineEdit, &QLineEdit::textEdited,
            this, &SaveGraphForm::validateGraphSave);

    connect(ui->graphSaveButton, &QPushButton::clicked,
            this, &SaveGraphForm::saveGraph);

    ui->nodeNodeValueRadio->click();
}



FileTypes::Type SaveGraphForm::getFileType()
{
    if (ui->nodeNodeValueRadio->isChecked())
    {
        if (ui->printValueCheck->isChecked())
            return FileTypes::Type::NODE_NODE_VALUE;
        else
            return FileTypes::Type::NODE_NODE;
    }
    else if (ui->bracketsFlatRadio->isChecked())
    {
        if (ui->printValueCheck->isChecked())
            return FileTypes::Type::BRACKETS_FLAT_VALUE;
        else
            return FileTypes::Type::BRACKETS_FLAT;
    }
    else if (ui->bracketsNestedRadio->isChecked())
    {
        if (ui->printValueCheck->isChecked())
            return FileTypes::Type::BRACKETS_VALUE;
        else
            return FileTypes::Type::BRACKETS;
    }
    return FileTypes::Type::UNDEFINED;
}



void SaveGraphForm::graphTypeChanged()
{
    if (ui->nodeNodeValueRadio->isChecked())
    {
        ui->printIndentsCheck->setEnabled(false);
        ui->startNodeLineEdit->setEnabled(false);
        ui->pathLimitLineEdit->setEnabled(false);
    }
    else if (ui->bracketsFlatRadio->isChecked())
    {
        ui->printIndentsCheck->setEnabled(false);
        ui->startNodeLineEdit->setEnabled(false);
        ui->pathLimitLineEdit->setEnabled(true);
    }
    else if (ui->bracketsNestedRadio->isChecked())
    {
        ui->printIndentsCheck->setEnabled(true);
        ui->startNodeLineEdit->setEnabled(true);
        ui->pathLimitLineEdit->setEnabled(true);
    }
    validateGraphSave();
}



/**
 * @brief SaveToFileGraphForm::validateGraphSave - This big function used for
 * validate save graph params and show file part example how it will looks.
 */
void SaveGraphForm::validateGraphSave()
{
    ui->graphSaveButton->setEnabled(false);

    FileTypes::Type typeId = getFileType();
    if (typeId == FileTypes::Type::UNDEFINED)
    {
        ui->infoLabel->setText(tr("File type is not recognised. "
                                  "Please check save settings"));
        return;
    }
    fileTypeId = typeId;

    bool printValue = ui->printValueCheck->isChecked();
    bool printInfo = ui->printInfoCheck->isChecked();

    QString info;

    if (mainWindow->getGraph().isGraphEmpty())
    {
        info += tr("Graph is empty. Nothing to save.");
        ui->infoLabel->setText(info);
        return;
    }

    if (!printValue && !printInfo)
    {
        info += tr("Print value and print info are turned off.\n"
                   "Weight and other information will be lost.");
        info += '\n';
    }

    bool printIndents  = ui->printIndentsCheck->isEnabled() &&
                         ui->printIndentsCheck->isChecked();

    const Graph* graph = &mainWindow->getGraph();

    if (ui->bracketsNestedRadio->isChecked())
    {
        if (!ui->startNodeLineEdit->hasAcceptableInput())
        {
            ui->infoLabel->setText(tr("Start node is not number."));
            return;
        }

        bool ok;
        ulong nodeId = ui->startNodeLineEdit->text().toULong(&ok);
        if (ok)
        {
            const Node* node = graph->findNode(nodeId);
            if (node)
            {
                nodeId = node->getId();
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
                ui->infoLabel->setText(info);
                return;
            }
        }
        else
        {
            info +=  tr("Start node is not number.");
            ui->infoLabel->setText(info);
            return;
        }
    }


    QString example;
    example += FileTypes::typeName(typeId) ;
    example += '\n';

    if (printInfo)
    {
        const InfoDeque* list = graph->getAllInfo();
        if (!list->size())
        {
            info += tr("Graph info is empty.") + '\n';
        }
        else
        for (auto it = list->begin(); it != list->end(); ++it)
        {
            example += '{';
            example += it->first;
            example += " = ";
            example += it->second;
            example += "}\n";
        }
    }
    if (ui->nodeNodeValueRadio->isChecked())
    {
        if (printValue)
        {
            example += "1 2 1\n1 3 1\n2 4 8\n2 5 4\n";
        }
        else
        {
            example += "1 2\n1 3\n2 4\n2 5\n";
        }
    }
    else if (ui->bracketsFlatRadio->isChecked())
    {
        if (printValue)
        {
            if (printIndents)
                example += "1(\n    2[5]\n    4[7]\n    5[9]\n)\n";
            else
                example += "1(2[5] 4[7] 5[9])\n";
        }
        else
        {
            if (printIndents)
                example += "1(\n    2\n    4\n    5\n)\n";
            else
                example += "1(2 4 5)\n";
        }
    }
    else if (ui->bracketsNestedRadio->isChecked())
    {
        if (printValue)
        {
            if (printIndents)
                example += "1(\n    2[5](\n        4[7]\n        5[9]\n"
                           "    )\n)\n";
            else
                example += "1(2[5](4[7] 5[9]))\n";
        }
        else
        {
            if (printIndents)
                example += "1(\n    2(\n        4\n        5\n    )\n)\n";
            else
                example += "1(2(4 5))\n";
        }
    }

    ui->infoLabel->setText(info);
    ui->exampleLabel->setText(example);
    ui->graphSaveButton->setEnabled(true);
}



void SaveGraphForm::saveGraph()
{
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
    unsigned options = WriterBase::Option::NONE;

    if (ui->printValueCheck->isChecked())
    {
        options = options | WriterBase::Option::PRINT_VALUE;
    }
    if (ui->printInfoCheck->isChecked())
    {
        options = options | WriterBase::Option::PRINT_INFO;
    }

    if (ui->printIndentsCheck->isEnabled() &&
            ui->printIndentsCheck->isChecked())
    {
        options = options | WriterBase::Option::PRINT_INDENTS;
    }

    GraphWorker& graph = mainWindow->getGraph();
    QByteArray file = fileName.toLatin1();

    bool result = false;
    size_t startNodeId, pathLimit;

    switch (fileTypeId)
    {
    case FileTypes::Type::NODE_NODE: // no break
    case FileTypes::Type::NODE_NODE_VALUE:
        result = graph.writeEdges(file.data(), options);
        break;

    case FileTypes::Type::BRACKETS_FLAT: // no break
    case FileTypes::Type::BRACKETS_FLAT_VALUE:
        result = graph.writeBracketsFlat(file.data(), options);
        break;

    case FileTypes::Type::BRACKETS: // no break
    case FileTypes::Type::BRACKETS_VALUE:
        startNodeId = ui->startNodeLineEdit->text().toUInt(&result);
        if (!result)
            break;
        pathLimit = ui->pathLimitLineEdit->text().toUInt(&result);
        if (!result)
            break;
        result = graph.writeBrackets(file.data(), startNodeId, pathLimit,
                                      options);
        break;

    default:
        ui->infoLabel->setText(tr("File type is unknown. Check settings."));
        break;
    }
    if (result)
    {
        int p = fileName.lastIndexOf(QDir::separator());
        QString shortName = fileName.mid(++p);
        mainWindow->showStatusMessage(tr("Graph saved in file %1").
                                    arg(shortName));
    }
    else
    {
        ui->infoLabel->setText(tr("Graph not saved."
                                  "File not accessed to write."));
        mainWindow->showStatusMessage(tr("Graph not saved."), 5000);
    }

}



