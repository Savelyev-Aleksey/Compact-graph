#include <thread>

#include "Projections.h"
#include "Projection.h"
#include "GraphBase.h"

Projections::Projections(GraphBase& graph) :
    Worker(),
    graph(&graph),
    projections(new ProjectionsList)
{}



Projections::~Projections()
{
    for (auto it = projections->rbegin(); it != projections->rend(); ++it)
    {
        delete it->second;
    }
    delete projections;
}



bool Projections::isEmpty() const
{
    return projections->size() == 0;
}



const ProjectionsList* Projections::getList() const
{
    return projections;
}



void Projections::createAllProjections()
{
    if (graph->isEmpty())
    {
        return;
    }
    // Prepare nodes
    NodeMap* list = graph->getNodeMap();
    size_t count = list->size();
    size_t end;
    auto it = list->begin();

    // prepare threads
    size_t threadsCount = std::thread::hardware_concurrency();
    if (!threadsCount)
        threadsCount = 2;

    std::thread* threads[threadsCount];

    startProcess(0, count - 1);

    auto func = [](Projection* pr, GraphBase* graph){
        pr->createProjection(*graph);
    };

    for (size_t i = 0; i < count; i += threadsCount)
    {
        if (isInterrupted())
            return;

        end = std::min(count - i, threadsCount);

        for (unsigned j = 0; j < end; ++j, ++it)
        {
            Projection* pr = new Projection(it->first);
            projections->insert({it->first, pr});

            threads[j] = new std::thread(func, pr, graph);
        }
        for (unsigned j = 0; j < end; ++j)
        {
            threads[j]->join();
            delete threads[j];
        }
        updateProgress(i + end);
    }
    completeProcess();
}



void Projections::createProjection(size_t nodeId)
{
    auto it = projections->find(nodeId);
    if (it != projections->end())
        return;
    Projection* pr = new Projection(it->first);
    projections->insert({it->first, pr});
    pr->createProjection(*graph);
}
