#include <thread>

#include "Projections.h"
#include "Projection.h"
#include "GraphBase.h"

Projections::Projections(GraphBase& graph) :
    Worker(),
    graph(&graph),
    projectionsList(new ProjectionsList)
{}



Projections::~Projections()
{
    clear();
    delete projectionsList;
}



void Projections::clear()
{
    for (auto it = projectionsList->rbegin();
         it != projectionsList->rend(); ++it)
    {
        delete it->second;
    }
    projectionsList->clear();
}



bool Projections::isEmpty() const
{
    return projectionsList->size() == 0;
}



const ProjectionsList* Projections::getList() const
{
    return projectionsList;
}



GraphBase& Projections::getGraph() const
{
    return *graph;
}



Projection* Projections::getProjection(size_t nodeId) const
{
    auto it = projectionsList->find(nodeId);
    return it == projectionsList->end() ? nullptr : it->second;
}



/**
 * @brief Projections::createAllProjections creates projection
 * for each node in graph.
 */
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

    std::thread* threads[threadsCount] {nullptr} ;

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
            auto result = projectionsList->insert({it->first, pr});
            if (result.second)
            {
                threads[j] = new std::thread(func, pr, graph);
            }
            else
            {
                delete pr;
            }
        }
        for (unsigned j = 0; j < end; ++j)
        {
            if (threads[j])
            {
                threads[j]->join();
                delete threads[j];
            }
            threads[j] = nullptr;
        }
        updateProgress(i + end);
    }
    completeProcess();
}



/**
 * @brief Projections::createProjection create projection for one node in
 * graph.
 * @param nodeId - node id wich will be respectived node.
 */
void Projections::createProjection(size_t nodeId)
{
    auto it = projectionsList->find(nodeId);
    if (it != projectionsList->end())
        return;
    Projection* pr = new Projection(nodeId);
    projectionsList->insert({nodeId, pr});
    pr->createProjection(*graph);
}



size_t Projections::size() const
{
    return projectionsList->size();
}



bool Projections::isProjectionExist(size_t nodeId) const
{
    auto it = projectionsList->find(nodeId);
    return (it != projectionsList->end()) ? true : false;
}



/**
 * @brief Projections::getEccentriciyStatistic - create eccentricity statistic.
 * Show eccentricity and mathed count in graph.
 * @return map with eccentricity and matched count.
 */
UlongMap* Projections::getEccentriciyStatistic() const
{
    if (!projectionsList->size())
    {
        return nullptr;
    }
    size_t eccentr;
    UlongMap* map = new UlongMap();

    for (const auto &it : *projectionsList)
    {
        eccentr = it.second->getEccentricity();
        auto result = map->insert({eccentr, 1});
        if (!result.second)
        {
            // Increase count of exist eccentricity
            ++(result.first->second);
        }
    }
    return map;
}



size_t Projections::getGraphGirth() const
{
    size_t min = SIZE_MAX, current;
    for (const auto &it : *projectionsList)
    {
        current = it.second->getShortestLoop();
        if (current < min)
        {
            min = current;
        }
        // it's minimal - no looks other
        if (min == 3)
            break;
    }
    return min;
}
