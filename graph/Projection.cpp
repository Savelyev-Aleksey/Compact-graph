#include <set>

#include "Projection.h"
#include "ProjectionElem.h"
#include "GraphBase.h"
#include "Node.h"
#include "Edge.h"



Projection::Projection(size_t nodeId) :
    nodeId(nodeId),
    rootNode(nullptr),
    levelList(nullptr),
    eccesntricity(0)
{}



Projection::~Projection()
{
    if (!levelList)
        return;
    for(size_t i = levelList->size(); i > 0 ; --i)
    {
        delete levelList->at(i - 1);
    }
    delete levelList;
    ProjectionElem* parent = rootNode;
    ProjectionElem* elem = rootNode;
    const ProjectionElemMap* list;
    // if reach root elem do unil it became leaf
    while(parent || !elem->isLeaf())
    {
        if (elem->isLeaf())
        {
            delete elem;
            elem = parent;
            list = elem->getList();
            elem->eraseElem(list->begin());
            parent = elem->getParent();
        }
        else
        {
            parent = elem;
            elem = elem->getList()->begin()->second;
        }
    }
    delete elem;
}



size_t Projection::getId() const
{
    return nodeId;
}



size_t Projection::levelCount() const
{
    return levelList->size();
}



const ProjectionElem* Projection::getRootNode() const
{
    return rootNode;
}



/**
 * @brief Projection::createProjection create projection for current
 * projection element
 * @param graph - graph used for generate projection
 */
void Projection::createProjection(GraphBase &graph)
{
    if (levelList)
        return;
    levelList = new ProjectionLevelList;
    // List of nodes to check, that all visited
    NodeIdSet* graphNodes = graph.getNodeIds();
    // init perspectived node, for projection it's a root node
    ProjectionElem* elem = new ProjectionElem(nodeId);
    rootNode = elem;
    // init zero level
    ProjectionLevelElem* curLevel = new ProjectionLevelElem;
    // add in zero level perspectived node
    curLevel->insert({nodeId, elem});
    // push level in level list
    levelList->push_back(curLevel);

    // Add map for original nodes - use for set link in replics to original node
    ProjectionElemMap* origianlNodes = new ProjectionElemMap;
    origianlNodes->insert({nodeId, elem});
    // Remove perspectived node - already in projection
    graphNodes->erase(nodeId);

    ProjectionLevelElem* parentLevel;
    size_t projectionLevel = 0;
    bool isComlpete = false;

    while (!isComlpete)
    {
        if (!graphNodes->size())
        {
            // All nodes in projection - createing complete
            // but continue last loop - if last level have edges between
            // nodes on last projection level
            isComlpete = true;
            // set eccesntricity - all nodes are visited (added in projection)
            eccesntricity = projectionLevel - 1;
        }
        parentLevel = levelList->at(projectionLevel);
        curLevel = new ProjectionLevelElem();
        bool isEmpty = true;

        for (const auto &parentIt : *parentLevel)
        {
            ProjectionElem* parent = parentIt.second;
            if (!parent->isOriginal())
                continue;

            isEmpty = false;

            const Node* node = graph.getNode(parentIt.first);
            const EdgeList* edgeList = node->getEdges();
            for (const auto &edgeIt : *edgeList)
            {
                size_t curId = edgeIt.first;
                // skip node if already it's parent in path
                if (parent->findInParents(curId))
                    continue;

                elem = new ProjectionElem(curId);
                // insert elem as child in parent elem
                parent->addElem(elem);
                elem->setParent(parent);
                // insert elem in current projection level list
                curLevel->insert({curId, elem});

                // insert original node in search list
                // if not inserted - it's replica node
                auto res = origianlNodes->insert({curId, elem});
                if (res.second)
                {
                    // new original node visited, shrink unvisited nodes
                    graphNodes->erase(curId);
                }
                else
                {
                    // set node as replica - set original node address
                    elem->setOriginal(res.first->second);
                }
            }

        }
        if (isEmpty)
        {
            // it's for last level if no original - just clear empty level
            delete curLevel;
        }
        else
        {
            // add new level in projection list
            levelList->push_back(curLevel);
        }
        ++projectionLevel;
    }
}

