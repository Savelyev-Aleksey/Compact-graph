
#include <deque>

#include "ShortPathRootElem.h"
#include "ShortPathElem.h"



ShortPathRootElem::ShortPathRootElem(size_t nodeId) :
    nodes(new ShortPathElem(nodeId, 0, 0)),
    search(new ShortPathElem(nodeId, 0, 0)),
    eccentricity(0),
    nodeId(nodeId)
{}



ShortPathRootElem::~ShortPathRootElem()
{
    clear();
    delete search;
    delete nodes;
}



size_t ShortPathRootElem::size() const
{
    return search->size();
}



/**
 * @brief ShortPath::clear - Remove nodes from hierarchy elem and clean
 * references in map.
 */
void ShortPathRootElem::clear()
{
    PathList *list = search->getPathList();
    if (!list || !list->size())
        return;

    for (auto &it : *list)
    {
        delete it.second;
    }
    list->clear();
    nodes->getPathList()->clear();
}



void ShortPathRootElem::updateEccentricity()
{
    const PathList* list = search->getPathList();
    if (!list)
    {
        return;
    }
    size_t eccentrMax = 0, eccentr;
    for (auto it = list->begin(), end = list->end(); it != end; ++it)
    {
        eccentr = it->second->getIndent();
        if (eccentr > eccentrMax)
            eccentrMax = eccentr;
    }
    eccentricity = eccentrMax;
}



ShortPathElem* ShortPathRootElem::getNodes() const
{
    return nodes;
}



ShortPathElem* ShortPathRootElem::getSearch() const
{
    return search;
}



void ShortPathRootElem::setEccentricity(size_t eccentricity)
{
    this->eccentricity = eccentricity;
}



size_t ShortPathRootElem::getEccentricity() const
{
    return eccentricity;
}



size_t ShortPathRootElem::getNodeId() const
{
    return nodeId;
}

