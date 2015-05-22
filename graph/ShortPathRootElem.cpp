
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
    list->clear();
    list = nodes->getPathList();

    if (list == nullptr || !list->size())
    {
        return;
    }

    std::deque <PathList *> stack;
    ShortPathElem* elem;
    PathList::const_iterator it;

    stack.push_front(list);

    while (stack.size())
    {
        // List exists and non empty
        if (stack.front() && stack.front()->size())
        {
            // Take first elem in list
            it = stack.front()->begin();
            elem = it->second;
            list = elem->getPathList();
            if (list && list->size())
            {
                // Save elem map in stack and continue -> enter in this map
                stack.push_front(list);
            }
            else
            {
                // Remove elem refence from structure
                stack.front()->erase(it);
                // Destroy elem
                delete elem;
            }
            continue;
        }
        stack.pop_front();
    }
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

