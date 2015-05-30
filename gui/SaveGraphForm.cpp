#include <vector>
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
    connect(ui->projectionRadio, &QRadioButton::clicked,
            this, &SaveGraphForm::graphTypeChanged);


    // Set validators
    ui->pathLimitLineEdit->setValidator(new QIntValidator(1, 100000, this));
    ui->startNodeLineEdit->setValidator(new QIntValidator(0, 100000, this));


    // each options change call validate slot
    connect(ui->printIndentsCheck, &QCheckBox::clicked,
            this, &SaveGraphForm::validateInputs);
    connect(ui->printInfoCheck,  &QCheckBox::clicked,
            this, &SaveGraphForm::validateInputs);
    connect(ui->printValueCheck, &QCheckBox::clicked,
            this, &SaveGraphForm::validateInputs);

    connect(ui->printAllNodesCheck, &QCheckBox::clicked,
            this, &SaveGraphForm::validateInputs);
    connect(ui->printAllNodesCheck, &QCheckBox::clicked,
            ui->startNodeLineEdit,  &QLineEdit::setDisabled);

    connect(ui->startNodeLineEdit, &QLineEdit::textEdited,
            this, &SaveGraphForm::validateInputs);
    connect(ui->pathLimitLineEdit, &QLineEdit::textEdited,
            this, &SaveGraphForm::validateInputs);

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
    else if (ui->projectionRadio->isChecked())
    {
        return FileTypes::Type::PROJECTIONS;
    }
    return FileTypes::Type::UNDEFINED;
}



void SaveGraphForm::graphTypeChanged()
{
    if (ui->nodeNodeValueRadio->isChecked())
    {
        ui->printValueCheck->setEnabled(true);
        ui->printInfoCheck->setEnabled(true);
        ui->printIndentsCheck->setEnabled(false);
        ui->printAllNodesCheck->setEnabled(false);

        ui->startNodeLineEdit->setEnabled(false);
        ui->pathLimitLineEdit->setEnabled(false);
    }
    else if (ui->bracketsFlatRadio->isChecked())
    {
        ui->printValueCheck->setEnabled(true);
        ui->printInfoCheck->setEnabled(true);
        ui->printIndentsCheck->setEnabled(false);
        ui->printAllNodesCheck->setEnabled(false);

        ui->startNodeLineEdit->setEnabled(false);
        ui->pathLimitLineEdit->setEnabled(false);
    }
    else if (ui->bracketsNestedRadio->isChecked())
    {
        ui->printValueCheck->setEnabled(true);
        ui->printInfoCheck->setEnabled(true);
        ui->printIndentsCheck->setEnabled(true);
        ui->printAllNodesCheck->setEnabled(false);

        ui->startNodeLineEdit->setEnabled(true);
        ui->pathLimitLineEdit->setEnabled(true);
    }
    else if (ui->projectionRadio->isChecked())
    {
        ui->printValueCheck->setEnabled(false);
        ui->printInfoCheck->setEnabled(true);
        ui->printIndentsCheck->setEnabled(true);
        ui->printAllNodesCheck->setEnabled(true);

        bool isAll = ui->printAllNodesCheck->isChecked();
        ui->startNodeLineEdit->setEnabled(!isAll);
        ui->pathLimitLineEdit->setEnabled(false);
    }
    validateInputs();
}



/**
 * @brief SaveGraphForm::validateNodeNodeValue - function is part of validation
 * used valide type node node value (edges list)
 * @param info - info string for add info about validation
 * @return true if validation successful
 */
bool SaveGraphForm::validateNodeNodeValue(QString& info)
{
    bool printValue = ui->printValueCheck->isChecked();
    bool printInfo = ui->printInfoCheck->isChecked();

    if (!printValue && !printInfo)
    {
        info += tr("Print value and print info are turned off.\n"
                   "Weight and other information will be lost.");
        info += '\n';
    }

    FileTypes::Type typeId = getFileType();
    QString example;
    example += FileTypes::typeName(typeId);
    example += '\n';

    if (printInfo)
    {
        example += "{TYPE=HYPERCUBE}\n";
        example += "{WEIGHT=1}\n";
    }
    if (printValue)
    {
        example += "1 2 1\n1 3 1\n2 4 8\n2 5 4\n";
    }
    else
    {
        example += "1 2\n1 3\n2 4\n2 5\n";
    }

    ui->exampleLabel->setText(example);

    return true;
}



/**
 * @brief SaveGraphForm::validateBracketsFlat - function is part of validation
 * used valide type flat brackets format (adjacency list)
 * @param info - info string for add info about validation
 * @return true if validation successful
 */
bool SaveGraphForm::validateBracketsFlat(QString& info)
{
    bool printValue = ui->printValueCheck->isChecked();
    bool printInfo = ui->printInfoCheck->isChecked();

    if (!printValue && !printInfo)
    {
        info += tr("Print value and print info are turned off.\n"
                   "Weight and other information will be lost.");
        info += '\n';
    }

    FileTypes::Type typeId = getFileType();
    QString example;
    example += FileTypes::typeName(typeId);
    example += '\n';

    if (printInfo)
    {
        example += "{TYPE=HYPERCUBE}\n";
        example += "{WEIGHT=1}\n";
    }

    if (printValue)
    {
        example += "1(2[5] 4[7] 5[9])\n";
    }
    else
    {
        example += "1(2 4 5)\n";
    }

    ui->exampleLabel->setText(example);

    return true;
}



/**
 * @brief SaveGraphForm::validateBracketsNested - function is part of validation
 * used valide type flat brackets format (bracket style all nodes)
 * @param info - info string for add info about validation
 * @param isGraphEmpty - if graph empty skip node search
 * @return true if validation successful
 */
bool SaveGraphForm::validateBracketsNested(QString& info, bool isGraphEmpty)
{
    bool printValue = ui->printValueCheck->isChecked();
    bool printInfo  = ui->printInfoCheck->isChecked();
    bool printIndents = ui->printIndentsCheck->isChecked();

    bool isValid = true;

    if (!printValue && !printInfo)
    {
        info += tr("Print value and print info are turned off.\n"
                   "Weight and other information will be lost.");
        info += '\n';
    }

    isValid = !isGraphEmpty && checkStartNodeEdit(info);

    FileTypes::Type typeId = getFileType();
    QString example;
    example += FileTypes::typeName(typeId);
    example += '\n';

    if (printInfo)
    {
        example += "{TYPE=HYPERCUBE}\n";
        example += "{WEIGHT=1}\n";
    }

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

    ui->exampleLabel->setText(example);

    return isValid;
}



bool SaveGraphForm::validateProjections(QString &info, bool isGraphEmpty)
{
    bool isValid = true;
    bool printAll = ui->printAllNodesCheck->isChecked();
    bool printIndents = ui->printIndentsCheck->isChecked();


    if (!isGraphEmpty)
    {
        const GraphWorker& graph = mainWindow->getGraph();
        if (printAll)
        {
            unsigned gSize = graph.nodeCount();
            unsigned pSize = graph.projectionsCount();
            if (gSize != pSize)
            {
                info += tr("Not all projections are created yet. "
                           "Will be created on save. It's take a time.");
                info += '\n';
            }
        }
        else
        {
            isValid = checkStartNodeEdit(info);
            if (isValid)
            {
                unsigned nodeId = ui->startNodeLineEdit->text().toULong();
                if (!graph.isProjectionExist(nodeId))
                {
                    info += tr("Projection not exist yet. "
                               "Will be created on save.");
                    info += '\n';
                }
            }
        }
    }

    QString example;
    FileTypes::Type typeId = getFileType();

    example += FileTypes::typeName(typeId);
    example += '\n';
    example += "{WEIGHT=1}\n";

    if (printIndents)
        example += "1(\n    2(\n        4\n        5\n        6\n    )\n)\n";
    else
        example += "1(2(4 5 6))\n";

    ui->exampleLabel->setText(example);

    return isValid;
}



/**
 * @brief SaveGraphForm::checkStartNodeEdit - check on exist node in graph.
 * If node not fonud print nearest this id other nodes.
 * @param info - pointer on info string for get info
 * @return true if node id correct and node exist
 */
bool SaveGraphForm::checkStartNodeEdit(QString &info)
{
    bool isValid = true;

    const GraphWorker& graph = mainWindow->getGraph();

    if (!ui->startNodeLineEdit->hasAcceptableInput())
    {
        info += tr("Start node is not number.");
        info += '\n';
        isValid = false;
    }
    else
    {
        bool ok;
        ulong nodeId = ui->startNodeLineEdit->text().toULong(&ok);
        if (ok)
        {
            const Node* node = graph.findNode(nodeId, info);
            if (!node)
            {
                isValid = false;
            }
        }
        else
        {
            info += tr("Start node is not number.");
            info += '\n';
            isValid = false;
        }
    }
    return isValid;
}



/**
 * @brief SaveGraphForm::validateInputs - This function used for
 * validate save graph params and show file part example how it will looks.
 */
void SaveGraphForm::validateInputs()
{
    FileTypes::Type typeId = getFileType();
    if (typeId == FileTypes::Type::UNDEFINED)
    {
        ui->infoLabel->setText(tr("File type is not recognised. "
                                  "Please check save settings"));
        return;
    }
    fileTypeId = typeId;

    bool isValid = true;

    bool isEmpty = mainWindow->getGraph().isGraphEmpty();

    QString info;

    if (isEmpty)
    {
        info += tr("Graph is empty. Nothing to save.");
        info += '\n';
        isValid = false;
    }

    if (ui->nodeNodeValueRadio->isChecked())
    {
        isValid = validateNodeNodeValue(info);
    }
    else if (ui->bracketsFlatRadio->isChecked())
    {
        isValid = validateBracketsFlat(info);
    }
    else if (ui->bracketsNestedRadio->isChecked())
    {
        isValid = validateBracketsNested(info, isEmpty);
    }
    else if (ui->projectionRadio->isChecked())
    {
        isValid = validateProjections(info, isEmpty);
    }

    ui->infoLabel->setText(info);

    ui->graphSaveButton->setEnabled(!isEmpty && isValid);
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
    unsigned options = (unsigned) WriterBase::Option::NONE;

    if (ui->printValueCheck->isEnabled() &&
        ui->printValueCheck->isChecked() )
    {
        options = options | (unsigned) WriterBase::Option::PRINT_VALUE;
    }
    if (ui->printInfoCheck->isEnabled() &&
        ui->printInfoCheck->isChecked() )
    {
        options = options | (unsigned) WriterBase::Option::PRINT_INFO;
    }

    if (ui->printIndentsCheck->isEnabled() &&
        ui->printIndentsCheck->isChecked() )
    {
        options = options | (unsigned) WriterBase::Option::PRINT_INDENTS;
    }

    bool printAllNodes = ui->printAllNodesCheck->isEnabled() &&
                         ui->printAllNodesCheck->isChecked();

    GraphWorker& graph = mainWindow->getGraph();
    QByteArray file = fileName.toLatin1();

    bool result = false;
    unsigned startNodeId, pathLimit;

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

    case FileTypes::Type::PROJECTIONS:
        if (printAllNodes)
        {
            graph.createAllProjections();
            if (graph.isInterrupted())
                result = false;
            else
                result = graph.saveProjections(file.data(), options);
        }
        else
        {
            startNodeId = ui->startNodeLineEdit->text().toUInt(&result);
            if (!result)
                break;
            graph.createProjection(startNodeId);
            result = graph.saveProjection(file.data(), startNodeId,options);
        }
        break;

    default:
        ui->infoLabel->setText(tr("File type is unknown. Check settings."));
        break;
    }

    if (graph.isInterrupted())
    {
        ui->infoLabel->setText(tr("Saving interrupted."));
        mainWindow->showStatusMessage(tr("Saving interrupted."), 5000);
    }
    else if (result)
    {
        int p = fileName.lastIndexOf(QDir::separator());
        QString shortName = fileName.mid(++p);
        mainWindow->showStatusMessage(tr("Graph saved in file %1").
                                    arg(shortName), 5000);
    }
    else
    {
        ui->infoLabel->setText(tr("Graph not saved."
                                  "File not accessed to write."));
        mainWindow->showStatusMessage(tr("Graph not saved."), 5000);
    }

}



