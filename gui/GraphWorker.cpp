#include <QWidget>
#include <QProgressDialog>
#include <thread>

#include "GraphWorker.h"
#include "graph/Node.h"



GraphWorker::GraphWorker(QWidget* parent) : Graph(),
    parent(parent)
{

}



GraphWorker::~GraphWorker()
{

}



void GraphWorker::progressDialog(size_t size, const QString& title,
                                 std::function<void ()> progressFn)
{
    QString titleP = title + QString(" %1");

    QProgressDialog progress(titleP.arg(0), QObject::tr("Cancel"), 0, 100,
                             parent);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(300);

    startProcess();
    std::thread creator(progressFn);
    creator.detach();

    size_t current;
    int value;
    do
    {
        if (progress.wasCanceled())
            interruptProcess();

        current = getProgress();
        value = current / (float) size * 100;
        progress.setValue(value);
        progress.setLabelText(titleP.arg(current));

    }
    while (isProcessed());
}



void GraphWorker::generateAllShortPaths()
{
    size_t size = graph->size();

    QString title = QObject::tr("Create short path");

    std::function<void()> fn = [this]{
        ::Graph::generateAllShortPaths();
    };

    progressDialog(size, title, fn);
}



bool GraphWorker::writeEdges(const char *fileName, cuint options)
{
    size_t size = graph->size();

    QString title = QObject::tr("Write edges of node");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &options, &result] {
        result = ::Graph::writeEdges(fileName, options);
    };
    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::writeBracketsFlat(const char *fileName, cuint options)
{
    size_t size = graph->size();

    QString title = QObject::tr("Write adjacency table");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &options, &result] {
        result = ::Graph::writeBracketsFlat(fileName, options);
    };

    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::writeBrackets(const char *fileName, const size_t startNodeId,
                                const size_t pathLimit, cuint options)
{
    size_t size = graph->getNode(startNodeId)->getEdgeCount() - 1;

    QString title = QObject::tr("Write adjacency table");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &startNodeId, &pathLimit,
                                &options, &result] {
        result = ::Graph::writeBrackets(fileName, startNodeId, pathLimit,
                                        options);
    };

    progressDialog(size, title, fn);
    return result;
}





