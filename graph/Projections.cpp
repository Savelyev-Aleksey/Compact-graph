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
    clear();
    delete projections;
}



void Projections::clear()
{
    for (auto it = projections->rbegin(); it != projections->rend(); ++it)
    {
        delete it->second;
    }
    projections->clear();
}



bool Projections::isEmpty() const
{
    return projections->size() == 0;
}



const ProjectionsList* Projections::getList() const
{
    return projections;
}



const Projection* Projections::getProjection(size_t nodeId) const
{
    auto it = projections->find(nodeId);
    return it == projections->end() ? nullptr : it->second;
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
            auto result = projections->insert({it->first, pr});
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
            threads[j]->join();
            delete threads[j];
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
    auto it = projections->find(nodeId);
    if (it != projections->end())
        return;
    Projection* pr = new Projection(it->first);
    projections->insert({it->first, pr});
    pr->createProjection(*graph);
}



size_t Projections::size() const
{
    return projections->size();
}



bool Projections::isProjectionExist(size_t nodeId) const
{
    auto it = projections->find(nodeId);
    return (it != projections->end()) ? true : false;
}



/**
 * @brief Projections::getEccentriciyStatistic - create eccentricity statistic.
 * Show eccentricity and mathed count in graph.
 * @return map with eccentricity and matched count.
 */
UlongMap* Projections::getEccentriciyStatistic() const
{
    if (!projections->size())
    {
        return nullptr;
    }
    size_t eccentr;
    UlongMap* map = new UlongMap();

    for (const auto &it : *projections)
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
