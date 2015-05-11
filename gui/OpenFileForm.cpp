#include <QFile>
#include <QFileDialog>

#include "graph/FileTypes.h"
#include "graph/ReaderBase.h"

#include "OpenFileForm.h"
#include "ui_OpenFileForm.h"

OpenFileForm::OpenFileForm(MainWindow* parent) :
    QWidget(parent),
    ui(new Ui::OpenFileForm),
    mainWindow(parent)
{
    ui->setupUi(this);

    connect(ui->chooseFileButton, &QPushButton::clicked,
            this, &OpenFileForm::getFileInfo);
    connect(ui->openFileButton,   &QPushButton::clicked,
            this, &OpenFileForm::readFile);
}



OpenFileForm::~OpenFileForm()
{
    delete ui;
}



/**
 * @brief OpenFileForm::getFileInfo - slot for execute file open dialog
 * choose file to get info before read file.
 */
void OpenFileForm::getFileInfo()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Open file"), "", tr("Text files (*.txt)") );
    if (fileName.length())
    {
        int p = fileName.lastIndexOf(QDir::separator());
        QString name = fileName.mid(++p);
        bool isReaded = readFileInfo(fileName);
        if (isReaded)
        {
            this->fileName = fileName;
            ui->openFilePathLabel->setText(name);
            ui->openFileButton->setEnabled(true);
        }
    }
    else
    {
        ui->openFilePathLabel->setText(tr("No file"));
        ui->openFileInfo->clear();
        ui->openFileButton->setEnabled(false);
    }
}



/**
 * @brief OpenFileForm::readFileInfo - Read file info and print on form.
 * There are print file type, additional info, line count, file size.
 * @param fileName - file name to get info
 * @return true - if file have known file format
 */
bool OpenFileForm::readFileInfo(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        QString error = tr("Can't read file %1").arg(fileName);
        mainWindow->showStatusMessage(error);
        ui->openFileInfo->setText(tr("Can't read file. Please check that there "
                                     "is a right to read."));
        return false;
    }
    QByteArray typeIdStr;
    typeIdStr = file.readLine();
    FileTypes::Type typeId = FileTypes::typeId(typeIdStr.data());

    if (typeId == FileTypes::Type::UNDEFINED)
    {
        ui->openFileInfo->setText(tr("Can't read file. "
                                     "File type is not recognised."));
        return false;
    }
    QString info;
    info = tr("File type = %1\n").arg(QLatin1String(typeIdStr.trimmed()));

    size_t lines = 1;
    QByteArray text;

    while (!file.atEnd())
    {
        ++lines;
        text = file.readLine();
        if (text.at(0) == '{')
        {
            QByteArray str(text);
            str = str.toUpper().mid(1, str.indexOf("}") - 1);
            int pos = str.indexOf("=");
            QByteArray name = str.mid(0, pos).trimmed();
            QByteArray value = str.mid(pos + 1);
            info += name + " = " + value + "\n";
        }
    }
    info += "\n" + tr("Lines in file: %1\n").arg(lines);
    info += tr("File size: %1 bytes").arg(file.size());
    file.close();
    ui->openFileInfo->setText(info);
    return true;
}



void OpenFileForm::readFile()
{
    ui->openFileButton->setEnabled(false);
    Graph& graph = mainWindow->getGraph();
    const char* name = fileName.toLocal8Bit().data();
    bool isReaded = graph.readFile(name);
    if (isReaded)
    {
        mainWindow->setOpenFileName(fileName);
        int p = fileName.lastIndexOf(QDir::separator());
        QString shortName = fileName.mid(++p);
        ui->infoLabel->setText( tr("File \"%1\" readed").arg(shortName) );
        mainWindow->showStatusMessage(tr("File \"%1\" readed").arg(shortName),
                                      5000);
    }
    else
    {
        QString lastError;
        switch (graph.getLastError())
        {
        case ReaderBase::Error::READ :
            lastError = tr("Can't read file.");
            break;
        case ReaderBase::Error::TYPE :
            lastError = tr("File have unknown type.");
            break;
        case ReaderBase::Error::SYNTAX :
            lastError = tr("File have syntax error.");
            break;
        default:
            lastError = tr("Unknown error.");
            break;
        }
        mainWindow->showStatusMessage(lastError);
        ui->infoLabel->setText(lastError + ' ' + tr("\nCheck log file "
            "to get more information about error.\nBy default file graph.log "
            "located in same program directory.") );
    }
}


