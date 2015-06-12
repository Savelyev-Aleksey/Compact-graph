#include <thread>
#include <algorithm>

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
    for (auto it = projectionsList->rbegin();
         it != projectionsList->rend(); ++it)
    {
        delete *it;
    }
    delete projectionsList;
}



void Projections::clear()
{
    for (auto it = projectionsList->rbegin();
         it != projectionsList->rend(); ++it)
    {
        delete *it;
    }
    projectionsList->clear();
}



bool Projections::isEmpty() const
{
    return projectionsList->size() == 0u;
}



const ProjectionsList* Projections::getList() const
{
    return projectionsList;
}



GraphBase& Projections::getGraph() const
{
    return *graph;
}



/**
 * @brief Projections::getProjection find projection for nodeId
 * @param nodeId - node id for search projection
 * @return pointer on projections if found else nullptr.
 */
Projection* Projections::getProjection(unsigned nodeId) const
{
    if (!projectionsList->size())
        return nullptr;

    auto start = projectionsList->begin();
    auto end = projectionsList->end();
    auto it = std::lower_bound(start, end, nodeId, Projection::lessById);
    return (it != end && (*it)->getId() == nodeId) ? *it : nullptr;
}



/**
 * @brief Projections::createAllProjections creates projection
 * for each node in graph. Used on small graphs.
 */
void Projections::createAllProjections()
{
    if (graph->isEmpty())
    {
        return;
    }
    // Prepare nodes
    NodeMap* nodeList = graph->getNodeMap();
    unsigned count = nodeList->size();

    unsigned oldSize = projectionsList->size();
    bool isWasEmpty = !oldSize;

    projectionsList->resize(count, nullptr);
    projectionsList->shrink_to_fit();

    auto oldStart = projectionsList->begin();
    auto oldEnd = projectionsList->begin() + oldSize;


    // prepare threads
    unsigned threadsCount = std::thread::hardware_concurrency();
    if (!threadsCount)
        threadsCount = 2;

    std::thread* threads[threadsCount] {nullptr} ;



    auto func = [](Projection* pr, GraphBase* graph){
        // skip non empty projections
        if (!pr->levelCount())
            pr->createProjection(*graph);
    };

    auto it = nodeList->begin();
    startProcess(0u, count - 1);

    unsigned i = 0u, end, currentId;
    unsigned pos = oldSize;
    while (i < count)
    {
        if (isInterrupted())
        {
            projectionsList->resize(pos);
            break;
        }

        end = std::min(count - i, threadsCount);

        unsigned j = 0u;
        while (j < end && i < count)
        {
            currentId = it->first;
            // Projection exist - skip
            if (!isWasEmpty)
            {
                auto e = std::lower_bound(oldStart, oldEnd, currentId,
                                          Projection::lessById);
                if (e != oldEnd && (*e)->getId() == currentId)
                {
                    ++it;
                    ++i;
                    continue;
                }
            }

            Projection* pr = new Projection(currentId);
            (*projectionsList)[pos] = pr;
            threads[j] = new std::thread(func, pr, graph);
            ++pos;
            ++i;
            ++j;
            ++it;
        }
        for (j = 0u; j < end; ++j)
        {
            if (threads[j])
            {
                threads[j]->join();
                delete threads[j];
            }
            threads[j] = nullptr;
        }
        updateProgress(i);
    }
    if (!isWasEmpty)
    {
        auto end = projectionsList->end();
        std::sort(oldStart, end, Projection::less);
    }
    completeProcess();
}



/**
 * @brief Projections::createProjection create projection for one node in
 * graph.
 * @param nodeId - node id wich will be respectived node.
 */
Projection* Projections::createProjection(unsigned nodeId)
{
    if (!graph->getNode(nodeId))
        return nullptr;

    Projection* pr = getProjection(nodeId);
    if (pr)
    {
        if (!pr->isEmpty())
            return pr;
    }
    else
    {
        pr = new Projection(nodeId);
        auto begin = projectionsList->begin();
        auto end = projectionsList->end();
        auto pos = std::lower_bound(begin, end, nodeId, Projection::lessById);
        projectionsList->insert(pos, pr);
    }

    pr->createProjection(*graph);
    return pr;
}



unsigned Projections::size() const
{
    return projectionsList->size();
}



bool Projections::isProjectionExist(unsigned nodeId) const
{
    return getProjection(nodeId) ? true : false;
}



/**
 * @brief Projections::getEccentriciyStatistic - create eccentricity statistic.
 * Show eccentricity and mathed count in graph.
 * @return map with eccentricity and matched count.
 */
UintMap* Projections::getEccentriciyStatistic() const
{
    if (!projectionsList->size())
    {
        return nullptr;
    }
    unsigned eccentr;
    UintMap* map = new UintMap();

    for (const auto &it : *projectionsList)
    {
        eccentr = it->getEccentricity();
        auto result = map->insert({eccentr, 1});
        if (!result.second)
        {
            // Increase count of exist eccentricity
            ++(result.first->second);
        }
    }
    return map;
}



unsigned Projections::getGraphGirth() const
{
    unsigned min = UINT32_MAX, current;
    for (const auto &it : *projectionsList)
    {
        current = it->getShortestLoop();
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



/**
 * @brief Projections::findShortPaths finds paths between fromId and toId
 * nodes Functions returns all available not intersected paths same length.
 * @param fromId - node id for seach paths.
 * @param toId - node id for seatch paths
 * @param reverse - by default path order from root elem to find elem
 * @return nullptr if no paths else vector of paths. path is vector of ids.
 */
ProjShortPaths* Projections::findShortPaths(unsigned fromId, unsigned toId,
                                            bool reverse)
{
    Projection* pr = getProjection(fromId);
    if (!pr)
        return nullptr;
    return pr->findShortPaths(toId, reverse);
}
