
#include <cstddef>
#include <iostream>
#include <map>
#include <deque>


#include "ShortPath.h"
#include "Node.h"
#include "Edge.h"
#include "GraphBase.h"
#include "ShortPathElem.h"
#include "ShortPathRootElem.h"




ShortPath::ShortPath(GraphBase& graph) :
    graph(&graph),
    shortPathNodes(new RootPathList)
{ }



ShortPath::~ShortPath()
{
    for (auto it = shortPathNodes->begin(); it != shortPathNodes->end(); ++it)
    {
        delete it->second;
    }
    delete shortPathNodes;
}



bool ShortPath::isPathExist(size_t nodeId) const
{
    auto it = shortPathNodes->find(nodeId);
    return it != shortPathNodes->end();
}



GraphBase& ShortPath::getGraph() const
{
    return *graph;
}




RootPathList* ShortPath::getShortPathNodes()
{
    return shortPathNodes;
}



/**
 * @brief ShortPath::initShortPath If short path not exist init fitrst.
 * @param startNodeId - node id to init
 * @return iterator on ShortPath Element
 */
ShortPathRootElem* ShortPath::initShortPath(size_t startNodeId)
{
    auto it = shortPathNodes->find(startNodeId);
    if (it != shortPathNodes->end())
        return it->second;

    ShortPathRootElem *elem = new ShortPathRootElem(startNodeId);

    shortPathNodes->insert( std::pair <size_t, ShortPathRootElem*>
                            (startNodeId, elem) );
    return elem;
}



/**
 * @brief ShortPath::clearPath - Remove short path from list.
 * @param nodeId - node id from which short path beginning
 */
void ShortPath::clearPath(size_t nodeId)
{
    auto it = shortPathNodes->find(nodeId);
    if (it == shortPathNodes->end())
    {
        return;
    }
    delete it->second;
    shortPathNodes->erase(it);
}



/**
 * @brief ShortPath::clearAllPaths - Clean all exists path
 */
void ShortPath::clearAllPaths()
{
    for (auto it = shortPathNodes->begin(); it != shortPathNodes->end(); ++it)
    {
        delete it->second;
    }
    shortPathNodes->clear();
}



bool ShortPath::isEmpty() const
{
    return shortPathNodes->size() == 0;
}



size_t ShortPath::getShortPathCount() const
{
    return shortPathNodes->size();
}



/**
 * @brief ShortPath::getEccentriciyStatistic - create eccentricity statistic.
 * Show eccentricity and mathed count in graph.
 * @return map with eccentricity and matched count.
 */
UlongMap* ShortPath::getEccentriciyStatistic() const
{
    if (!shortPathNodes->size())
    {
        return nullptr;
    }
    size_t eccentr;
    UlongMap* map = new UlongMap();

    for (auto it = shortPathNodes->begin(), end = shortPathNodes->end();
         it != end; ++it)
    {
        eccentr = it->second->getEccentricity();
        auto result = map->insert(std::pair<size_t, size_t>(eccentr, 1));
        if (!result.second)
        {
            // Increase count of exist eccentricity
            ++(result.first->second);
        }
    }
    return map;
}



/**
 * @brief ShortPath::isNodeForVisit - Check is the node in list
 * @param list - list where check node
 * @param nodeId - node id
 * @return true if node in list, false if not
 */
bool ShortPath::isNodeForVisit(const NodeDeque& list, size_t nodeId)
const
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if ((*it)->getId() == nodeId)
        {
            return true;
        }
    }
    return false;
}



/**
 * @brief ShortPath::updateParams - update current weight, indent level and
 * update this parameters for childs.
 * @param difference - difference beetwen old and new weight
 * @param indent - new element indent
 */
void ShortPath::updateParams(ShortPathElem* elem, float difference,
                             size_t indent)
{
    std::deque <ShortPathElem *> elemList;
    elemList.push_front(elem);
    float old = elem->getWeight();
    size_t oldIndent = elem->getIndent();
    size_t indentDiff;
    bool negative = false;
    if (indent > oldIndent)
    {
        indentDiff = indent - oldIndent;
    }
    else
    {
        indentDiff = oldIndent - indent;
        negative = true;
    }

    elem->setWeight(old - difference);

    while (elemList.size())
    {
        PathList* list = elemList.back()->getPathList();
        elemList.pop_back();
        if (!list)
        {
            continue;
        }
        for (auto it = list->begin(); it != list->end(); ++it)
        {
            ShortPathElem* elem = it->second;
            old = elem->getWeight();
            elem->setWeight(old - difference);
            oldIndent = elem->getIndent();
            if (negative)
                elem->setIndent(oldIndent - indentDiff);
            else
                elem->setIndent(oldIndent + indentDiff);
            elemList.push_front(elem);
        }
    }
}



void ShortPath::generateAllShortPaths(float pathLimit)
{
    if (graph->isEmpty())
    {
        return;
    }
    NodeMap* list = graph->getNodeMap();
    for (auto it = list->begin(), end = list->end(); it != end; ++it)
    {
        generateShortPath(it->first, pathLimit);
    }
}



/**
 * @brief ShortPath::generateShortPath - Generate short path in graph starts
 * from startNodeId node
 * @param startNodeId - Node id from which start short path
 * @param pathLimit - set max weight for path search
 */
void ShortPath::generateShortPath(const size_t startNodeId, float pathLimit)
{
    NodeMap* nodeList = graph->getNodeMap();
    // Check exists node
    auto nodeIt = nodeList->find(startNodeId);
    if (nodeIt == nodeList->end())
    {
        return;
    }

    bool pathExists= shortPathNodes->find(startNodeId) != shortPathNodes->end();
    if (pathExists)
    {
        return;
    }

    ShortPathRootElem* rootElem = initShortPath(startNodeId);

    NodeDeque nodesToVisit;
    nodesToVisit.push_front(nodeIt->second);

    createPaths(nodesToVisit, rootElem, pathLimit);
    rootElem->updateEccentricity();
}



void ShortPath::createPaths(NodeDeque& nodesToVisit,
                            ShortPathRootElem* rootElem, float pathLimit)
{
    Node* node;
    size_t fromNodeId, currentId;
    float weight;
    bool isFirst = true;
    const EdgeList* edgeList;

    size_t startId = rootElem->getNodeId();
    // Using for keep short path hierarchy
    ShortPathElem* parent = rootElem->getNodes();
    // Using for keep short path node in one dimention to fast access
    ShortPathElem* search = rootElem->getSearch();
    float baseWeight = 0;
    size_t indent = 1;

    while (nodesToVisit.size())
    {
        // Get last elem from queue
        node = nodesToVisit.back();
        nodesToVisit.pop_back();

        fromNodeId = node->getId();
        edgeList = node->getEdges();
        // First elem have no parent and no weight
        if (!isFirst)
        {
            // Get parent pointer where placed node
            parent = search->findElem(fromNodeId);
            // Find in parent node and set node by parent for add child elems.
            parent = parent->findElem(fromNodeId);
            baseWeight = parent->getWeight();
            // Level for next inserted element
            indent = parent->getIndent() + 1;
        }
        isFirst = false;

        for (auto edgeIt = edgeList->begin(), edgeEnd = edgeList->end();
             edgeIt != edgeEnd; ++edgeIt)
        {
            // Take node id
            currentId = edgeIt->first;
            if (currentId == startId)
            {
                continue;
            }
            weight = baseWeight + edgeIt->second->getValue();
            if (pathLimit && weight > pathLimit)
            {
                continue;
            }
            // Get elem parent for seek node
            ShortPathElem* oldParent = search->findElem(currentId);

            // Node not exists yet
            if (oldParent == nullptr)
            {
                // Add node hierarchy
                parent->addNodeElem(currentId, weight, indent);
                // add node parent in search map
                search->addNodeElem(currentId, parent);

                if ( !isNodeForVisit(nodesToVisit, currentId) )
                {
                    node = edgeIt->second->nodeElem(currentId);
                    nodesToVisit.push_front(node);
                }
                continue;
            }
            // Update node element
            // Find element in old parent
            ShortPathElem* elem = oldParent->findElem(currentId);
            if (elem->getWeight() > weight)
            {
                float difference = elem->getWeight() - weight;
                // Remove pointer from old parent
                oldParent->popNodeElem(currentId);
                // Insert into new parent
                parent->addNodeElem(currentId, elem);
                // Update parent in search map
                search->updateParent(currentId, parent);
                updateParams(elem, difference, indent);
            }
        }
    }

}

