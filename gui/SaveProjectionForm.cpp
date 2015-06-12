#include <QFileDialog>

#include "SaveProjectionForm.h"
#include "ui_SaveProjectionForm.h"
#include "MainWindow.h"



SaveProjectionForm::SaveProjectionForm(MainWindow* mainWindow, QWidget *parent):
    QWidget(parent),
    ui(new Ui::SaveProjectionForm),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    prepareWidgets();
    ui->projectionRadio->click();
}



SaveProjectionForm::~SaveProjectionForm()
{
    delete ui;
}



void SaveProjectionForm::prepareWidgets()
{
    // File type radio connect
    connect(ui->projectionRadio, &QRadioButton::clicked,
            this, &SaveProjectionForm::typeChanged);
    connect(ui->shortPathRadio, &QRadioButton::clicked,
            this, &SaveProjectionForm::typeChanged);


    // Set validators
    ui->startNodeLineEdit->setValidator(new QIntValidator(0, 100000, this));
    ui->secondNodeLineEdit->setValidator(new QIntValidator(0, 100000, this));


    // each options change call validate slot
    connect(ui->printIndentsCheck, &QCheckBox::clicked,
            this, &SaveProjectionForm::validateInputs);
    connect(ui->printInfoCheck,  &QCheckBox::clicked,
            this, &SaveProjectionForm::validateInputs);

    connect(ui->printAllNodesCheck, &QCheckBox::clicked,
            this, &SaveProjectionForm::validateInputs);
    connect(ui->printAllNodesCheck, &QCheckBox::clicked,
            ui->startNodeLineEdit,  &QLineEdit::setDisabled);

    connect(ui->startNodeLineEdit, &QLineEdit::textEdited,
            this, &SaveProjectionForm::validateInputs);
    connect(ui->secondNodeLineEdit, &QLineEdit::textEdited,
            this, &SaveProjectionForm::validateInputs);

    connect(ui->saveButton, &QPushButton::clicked,
            this, &SaveProjectionForm::saveButtonClicked);
}



void SaveProjectionForm::typeChanged()
{
    if (ui->projectionRadio->isChecked())
    {
        ui->printInfoCheck->setEnabled(true);
        ui->printIndentsCheck->setEnabled(true);
        ui->printAllNodesCheck->setEnabled(true);
        ui->reverseCheck->setEnabled(false);

        bool isAll = ui->printAllNodesCheck->isChecked();
        ui->startNodeLineEdit->setEnabled(!isAll);

        ui->secondNodeLineEdit->setEnabled(false);
    }
    else if (ui->shortPathRadio->isChecked())
    {
        ui->printInfoCheck->setEnabled(false);
        ui->printIndentsCheck->setEnabled(false);
        ui->printAllNodesCheck->setEnabled(false);
        ui->reverseCheck->setEnabled(true);
        ui->startNodeLineEdit->setEnabled(true);
        ui->secondNodeLineEdit->setEnabled(true);
    }
    validateInputs();
}



bool SaveProjectionForm::validateProjections(QString &info, bool isGraphEmpty)
{
    bool isValid = true;
    bool printAll = ui->printAllNodesCheck->isChecked();
    bool printIndents = ui->printIndentsCheck->isChecked();


    if (!isGraphEmpty)
    {
        const GraphWorker& graph = mainWindow->getGraph();
        if (printAll)
        {
            if (graph.isProjectionsMemoryUsed())
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
                info += tr("Projections are to large.");
                info += '\n';

                if(!graph.getFileName().length())
                {
                    // Graph not saved. Big projections can't be saved.
                    isValid = false;
                    info += tr("Graph not saved yet. Before save projections "
                               "save graph.");
                }
                info += ' ';
                info += tr("Each projection will be saved in own file "
                           "near graph file.");
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

    if (printIndents)
        example += "1(\n    2(\n        4\n        5\n        6\n    )\n)\n";
    else
        example += "1(2(4 5 6))\n";

    ui->exampleLabel->setText(example);

    return isValid;
}



/**
 * @brief SaveProjectionForm::checkStartNodeEdit - check on exist node in graph.
 * If node not fonud print nearest this id other nodes.
 * @param info - pointer on info string for get info
 * @return true if node id correct and node exist
 */
bool SaveProjectionForm::checkStartNodeEdit(QString &info)
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



bool SaveProjectionForm::checkSecondNodeEdit(QString &info)
{
    bool isValid = true;

    const GraphWorker& graph = mainWindow->getGraph();

    if (!ui->secondNodeLineEdit->hasAcceptableInput())
    {
        info += tr("Second node is not number.");
        info += '\n';
        isValid = false;
    }
    else
    {
        bool ok;
        ulong nodeId = ui->secondNodeLineEdit->text().toULong(&ok);
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
            info += tr("Second node is not number.");
            info += '\n';
            isValid = false;
        }
    }
    return isValid;
}



/**
 * @brief SaveProjectionForm::validateInputs - This function used for
 * validate save graph params and show file part example how it will looks.
 */
void SaveProjectionForm::validateInputs()
{
    bool isValid = true;

    bool isEmpty = mainWindow->getGraph().isGraphEmpty();

    QString info;

    if (isEmpty)
    {
        info += tr("Graph is empty. Nothing to save.");
        info += '\n';
        isValid = false;
    }

    if (ui->projectionRadio->isChecked())
    {
        isValid = validateProjections(info, isEmpty);
    }
    else if (ui->shortPathRadio->isChecked())
    {
        isValid = validateShortPath(info, isEmpty);
    }

    ui->infoLabel->setText(info);

    ui->saveButton->setEnabled(!isEmpty && isValid);
}



void SaveProjectionForm::saveProjection()
{
    unsigned options = (unsigned) WriterBase::Option::NONE;

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

    bool result = false;
    unsigned startId;
    QString fileName;
    QByteArray file;

    // For all projections in big graph create projections function
    // save each projection in own file
    if (!ui->printAllNodesCheck->isChecked() || graph.isProjectionsMemoryUsed())
    {
        fileName = QFileDialog::getSaveFileName(this, tr("Save graph"),
            QString(), tr("Text files (*.txt)"));

        if (fileName.isEmpty())
        {
            return;
        }
        if (!fileName.endsWith(".txt"))
        {
            fileName += ".txt";
        }
        file = fileName.toLocal8Bit();
    }


    if (printAllNodes)
    {
        if (graph.isProjectionsMemoryUsed())
        {
            // for small graph create in memory projections
            // and save all in one file
            graph.createAllProjections();
            result = graph.saveProjections(file.data(), options);
        }
        else
        {
            // If graph to big, saving called in create procedure
            // each projection will saved in own file
            graph.createAllProjections();
            result = graph.isInterrupted() ? false : true;
        }

    }
    else
    {
        // save one projection
        // projection will created again, even in big projection file
        // already exist.
        startId = ui->startNodeLineEdit->text().toUInt();
        auto pr = graph.createProjection(startId);
        result = graph.saveProjection(file.data(), pr,options);
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
        mainWindow->showStatusMessage(tr("Data saved in file %1").
                                    arg(shortName), 5000);
    }
    else
    {
        ui->infoLabel->setText(tr("Saving interrupted.") + ' ' +
                               tr("File not accessed to write."));
        mainWindow->showStatusMessage(tr("Saving interrupted."), 5000);
    }
}



bool SaveProjectionForm::validateShortPath(QString &info, bool isGraphEmpty)
{
    bool isValid = false;
    if (!isGraphEmpty)
    {
        isValid = checkStartNodeEdit(info);
        isValid = isValid && checkSecondNodeEdit(info);
    }
    ui->exampleLabel->setText("");
    return isValid;
}



void SaveProjectionForm::saveShortPath()
{
    unsigned startId, findId;
    bool reverse;
    GraphWorker& graph = mainWindow->getGraph();

    startId = ui->startNodeLineEdit->text().toUInt();
    findId = ui->secondNodeLineEdit->text().toUInt();
    reverse = ui->reverseCheck->isChecked();

    ProjShortPaths* paths = graph.findShortPaths(startId, findId, reverse);

    if (!paths)
    {
        ui->infoLabel->setText(tr("Short paths not found."));
        return;
    }

    QString fileName;
    QByteArray file;

    fileName = QFileDialog::getSaveFileName(this, tr("Save graph"),
        QString(), tr("Text files (*.txt)"));

    if (fileName.isEmpty())
    {
        return;
    }
    if (!fileName.endsWith(".txt"))
    {
        fileName += ".txt";
    }
    file = fileName.toLocal8Bit();

    bool result = graph.saveProjectionShortPaths(file.data(), paths);
    if (result)
    {
        mainWindow->showStatusMessage(tr("Short paths was saved."), 5000);
        ui->infoLabel->setText(tr("Short paths was saved."));
    }
    else
    {
        ui->infoLabel->setText(tr("Can't save file. Not accessed to write."));
    }
}



void SaveProjectionForm::saveButtonClicked()
{
    if (ui->projectionRadio->isChecked())
    {
        saveProjection();
    }
    else if (ui->shortPathRadio->isChecked())
    {
        saveShortPath();
    }
}
