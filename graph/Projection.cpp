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
    clear();
}



void Projection::clear()
{
    if (!levelList)
        return;
    for(size_t i = levelList->size(); i > 0 ; --i)
    {
        delete levelList->at(i - 1);
    }
    delete levelList;
    levelList = nullptr;

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
    rootNode = nullptr;
    eccesntricity = 0;
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



size_t Projection::getEccentricity() const
{
    return eccesntricity;
}



void Projection::setProjection(ProjectionElem *rootNode,
                               ProjectionLevelList *levelList)
{
    if (!rootNode || !levelList)
        return;

    clear();
    this->rootNode = rootNode;
    this->levelList = levelList;
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
    ProjectionElemMap origianlNodes;
    origianlNodes.insert({nodeId, elem});
    // Remove perspectived node - already in projection
    graphNodes->erase(nodeId);

    ProjectionLevelElem* parentLevel;
    size_t projectionLevel = 0;
    bool isComlpete = false;

    while (!isComlpete)
    {
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

                // insert elem as child in parent elem
                elem = parent->addElem(curId);
                // insert elem in current projection level list
                curLevel->insert({curId, elem});

                // insert original node in search list
                // if not inserted - it's replica node
                auto res = origianlNodes.insert({curId, elem});
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
            ++projectionLevel;
        }

        if (isEmpty || !graphNodes->size())
        {
            // All nodes in projection - creating complete.
            // If empty - so graph have to separate components.
            // One of components was full visited
            isComlpete = true;
            // set eccesntricity - all nodes are visited (added in projection)
            eccesntricity = projectionLevel;
        }
    }
    delete graphNodes;

    createLastLevelProjection(graph);
}



void Projection::createLastLevelProjection(const GraphBase& graph)
{
    // Last level find original elements on last level
    // which have edges between together
    std::deque <ProjectionElem*> nodes;
    ProjectionLevelElem* parentLevel = levelList->back();
    for (const auto &parentIt : *parentLevel)
    {
        if (parentIt.second->isOriginal())
        {
            nodes.push_back(parentIt.second);
        }
    }
    // If more one original nodes in last level
    // need to add new level for add edge info between together
    if (nodes.size() > 1)
    {
        size_t size = nodes.size();
        ProjectionLevelElem* curLevel = new ProjectionLevelElem;
        for(size_t i = 0; i < size - 1; ++i)
        {
            ProjectionElem* elem = nodes.at(i);
            for (size_t j = i + 1; j < size; ++j)
            {
               ProjectionElem* elem2 = nodes.at(j);
               if ( graph.getEdge(elem->getId(), elem2->getId()) )
               {
                   // Add second elem in first
                   size_t curId = elem2->getId();
                   // insert elem as child in parent elem
                   ProjectionElem* el = elem->addElem(curId);
                   el->setOriginal(elem2);
                   // insert elem in current projection level list
                   curLevel->insert({curId, el});

                   // Add first elem in second
                   curId = elem->getId();
                   // insert elem as child in parent elem
                   el = elem2->addElem(curId);
                   el->setOriginal(elem);
                   // insert elem in current projection level list
                   curLevel->insert({curId, el});
               }
            }
        }
        if (curLevel->size())
        {
            levelList->push_back(curLevel);
        }
        else
        {
            delete curLevel;
        }
    }
}



/**
 * @brief Projection::updateOriginalInfo updates info about original nodes.
 * Usual used after read projections from file. Where difficult store original
 * info about nodes. After reading start this function for update this info
 */
void Projection::updateOriginalInfo()
{
    if (!levelList)
        return;
    ProjectionElemMap origianlNodes;

    for(const auto &level : *levelList)
    {
        for (const auto &it : *level)
        {
            auto res = origianlNodes.insert({it.first, it.second});
            if (!res.second)
            {
                // set node as replica - set original node address
                it.second->setOriginal(res.first->second);
            }
        }
    }
}



void Projection::updateEccesntricity()
{
    if (!levelList)
        return;

    ProjectionLevelElem* level = levelList->back();
    for (const auto &it : *level)
    {
        // If one node is original so all original nodes on this level
        // have no edges between together
        if (it.second->isOriginal())
        {
            eccesntricity = levelList->size() - 1;
            return;
        }
    }
    // If no one original node on last level - all replicas
    // So last level only for add info about edges between original nodes
    // from penultimate level
    eccesntricity = levelList->size() - 2;
}
