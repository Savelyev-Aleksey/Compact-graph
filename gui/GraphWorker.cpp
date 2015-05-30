#include <QWidget>
#include <QString>
#include <QProgressDialog>
#include <thread>

#include "GraphWorker.h"
#include "graph/Node.h"
#include "graph/Projection.h"



GraphWorker::GraphWorker(QWidget* parent) : Graph(),
    parent(parent)
{

}



GraphWorker::~GraphWorker()
{

}



void GraphWorker::progressDialog(unsigned size, const QString& title,
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

    unsigned current;
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
    unsigned size = graph->nodeCount();
    if (!size)
        return;
    --size;

    QString title = QObject::tr("Creating short path");

    std::function<void()> fn = [this]{
        ::Graph::generateAllShortPaths();
    };

    progressDialog(size, title, fn);
}



void GraphWorker::createAllProjections()
{
    unsigned size = graph->nodeCount();
    if (!size)
        return;
    --size;

    QString title = QObject::tr("Creating projections");

    std::function<void()> fn = [this]{
        ::Graph::createAllProjections();
    };

    progressDialog(size, title, fn);
}



bool GraphWorker::writeEdges(const char *fileName, cuint options)
{
    unsigned size = graph->nodeCount();

    QString title = QObject::tr("Writing edges of node");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &options, &result] {
        result = ::Graph::writeEdges(fileName, options);
    };
    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::writeBracketsFlat(const char *fileName, cuint options)
{
    unsigned size = graph->nodeCount();

    QString title = QObject::tr("Writing adjacency list");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &options, &result] {
        result = ::Graph::writeBracketsFlat(fileName, options);
    };

    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::writeBrackets(const char *fileName, cuint startNodeId,
                                cuint pathLimit, cuint options)
{
    unsigned size = graph->getNode(startNodeId)->getEdgeCount();
    if (!size)
        return false;
    --size;

    QString title = QObject::tr("Writing adjacency list");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &startNodeId, &pathLimit,
                                &options, &result] {
        result = ::Graph::writeBrackets(fileName, startNodeId, pathLimit,
                                        options);
    };

    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::saveProjections(const char *fileName, cuint options)
{
    unsigned size = projectionsCount();
    if (!size)
        return false;
    --size;

    QString title = QObject::tr("Writing projections");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &options, &result] {
        result = ::Graph::saveProjections(fileName, options);
    };

    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::saveProjection(const char *fileName, cuint rootNode,
                                 cuint options)
{
    const Projection* pr = getProjection(rootNode);
    if (!pr)
    {
        return false;
    }
    unsigned size = pr->levelCount();
    if (!size)
        return false;
    --size;

    QString title = QObject::tr("Writing projection");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &rootNode, &options, &result] {
        result = ::Graph::saveProjection(fileName, rootNode, options);
    };

    progressDialog(size, title, fn);
    return result;
}



/**
 * @brief GraphWorker::findNode - find node in graph. if node does not exist
 * in info printing nearest nodes for this id.
 * @param nodeId - node id for search
 * @param info - info string to add info if node not found.
 * @return node pointer if found else nullptr
 */
const Node* GraphWorker::findNode(unsigned nodeId, QString& info) const
{
    const Node* node = ::Graph::findNode(nodeId);
    if (node)
        return node;
    info += QObject::tr("Node %1 not found.").arg(nodeId);
    info += ' ';
    uint_vec *nodes = findNearNode(nodeId);
    if (nodes)
    {
        info += QObject::tr("Nearest:");
        info += ' ';
        for(unsigned i = 0; i < nodes->size(); ++i)
        {
            if (i > 0)
                info += ", ";
            info += QString::number(nodes->at(i));
        }
        delete nodes;
    }
    return nullptr;
}





