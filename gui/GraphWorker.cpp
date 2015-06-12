#include <QWidget>
#include <QString>
#include <QProgressDialog>
#include <thread>
#include <iostream>

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
    if (!size)
        return;

    QString titleP = title + QString(" %1");

    QProgressDialog progressDg(titleP.arg(0), QObject::tr("Cancel"),0,INT32_MAX,
                             parent);

    progressDg.setWindowModality(Qt::WindowModal);
    progressDg.setMinimumDuration(800);

    startProcess();
    std::thread creator(progressFn);
    creator.detach();

    unsigned current;
    int value;

    progressDg.show();
    do
    {
        if (progressDg.wasCanceled())
            interruptProcess();

        current = getProgress();
        value = static_cast<int>(static_cast<double>(current)/size *INT32_MAX);

        progressDg.setValue(value);
        progressDg.setLabelText(titleP.arg(current));

    }
    while (isProcessed());
    progressDg.close();
}



void GraphWorker::createAllShortPaths()
{
    unsigned size = graph->nodeCount();
    if (!size)
        return;
    --size;

    QString title = QObject::tr("Creating short path");

    std::function<void()> fn = [this]{
        this->Graph::createAllShortPaths();
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
        this->Graph::createAllProjections();
    };

    progressDialog(size, title, fn);
}



bool GraphWorker::writeEdges(const char *fileName, cuint options)
{
    unsigned size = graph->nodeCount();

    QString title = QObject::tr("Writing edges of node");
    bool result = false;

    std::function<void()> fn = [this, &fileName, &options, &result] {
        result = this->Graph::writeEdges(fileName, options);
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
        result = this->Graph::writeBracketsFlat(fileName, options);
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
        result = this->Graph::writeBrackets(fileName, startNodeId, pathLimit,
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
        result = this->Graph::saveProjections(fileName, options);
    };

    progressDialog(size, title, fn);
    return result;
}



bool GraphWorker::saveProjection(const char *fileName, const Projection* pr,
                                 cuint options)
{
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

    std::function<void()> fn = [this, &fileName, &pr, &options, &result] {
        result = this->Graph::saveProjection(fileName, pr, options);
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
    const Node* node = Graph::findNode(nodeId);
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



ProjShortPaths* GraphWorker::findShortPaths(unsigned fromId, unsigned toId,
                                            bool reverse)
{
    QString title = QObject::tr("Finding short paths");
    ProjShortPaths* result = nullptr;

    std::function<void()> fn = [this, &fromId, &toId, &reverse, &result] {
        result = this->Graph::findShortPaths(fromId, toId, reverse);
    };
    progressDialog(1, title, fn);
    return result;
}



void GraphWorker::readProjectionsInfo()
{
    QString title = QObject::tr("Reading projections info");

    unsigned size = nodeCount();
    if (!size)
        return;
    --size;

    std::function<void()> fn = [this] {
        this->Graph::readProjectionsInfo();
    };
    progressDialog(size, title, fn);
}

