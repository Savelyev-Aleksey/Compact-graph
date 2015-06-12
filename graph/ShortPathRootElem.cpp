
#include <deque>

#include "ShortPathRootElem.h"
#include "ShortPathElem.h"



ShortPathRootElem::ShortPathRootElem(unsigned nodeId) :
    nodes(new ShortPathElem(nodeId, 0, 0u)),
    search(new ShortPathElem(nodeId, 0, 0u)),
    eccentricity(0u),
    nodeId(nodeId)
{}



ShortPathRootElem::~ShortPathRootElem()
{
    clear();
    delete search;
    delete nodes;
}



unsigned ShortPathRootElem::size() const
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
    unsigned eccentrMax = 0u, eccentr;
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



void ShortPathRootElem::setEccentricity(unsigned eccentricity)
{
    this->eccentricity = eccentricity;
}



unsigned ShortPathRootElem::getEccentricity() const
{
    return eccentricity;
}



unsigned ShortPathRootElem::getNodeId() const
{
    return nodeId;
}

