#include <set>
#include <cstdint>
#include <algorithm>

#include "Projection.h"
#include "ProjectionElem.h"
#include "GraphBase.h"
#include "Node.h"
#include "Edge.h"



Projection::Projection(unsigned nodeId) :
    nodeId(nodeId),
    rootNode(nullptr),
    levelList(nullptr),
    eccesntricity(0),
    shortestLoop(0)
{}



Projection::~Projection()
{
    clear();
}



void Projection::clear()
{
    if (!levelList)
        return;
    for(unsigned i = levelList->size(); i > 0 ; --i)
    {
        auto *level = levelList->at(i - 1);
        for (auto &it : *level)
        {
            delete it;
        }
        delete level;
    }
    delete levelList;
    levelList = nullptr;
    rootNode = nullptr;
    eccesntricity = 0;
    shortestLoop = 0;
}



bool Projection::less(const Projection *a, const Projection *b)
{
    return a->nodeId < b->nodeId;
}



bool Projection::lessById(const Projection *a, const unsigned &id)
{
    return a->nodeId < id;
}



unsigned Projection::getId() const
{
    return nodeId;
}



unsigned Projection::levelCount() const
{
    return levelList->size();
}



const ProjectionElem* Projection::getRootNode() const
{
    return rootNode;
}



unsigned Projection::getEccentricity() const
{
    return eccesntricity;
}



unsigned Projection::getShortestLoop() const
{
    return shortestLoop;
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
    elem->setLevel(0);
    rootNode = elem;
    // init zero level
    ProjectionLevelElem* curLevel = new ProjectionLevelElem;
    // add in zero level perspectived node
    curLevel->push_back(elem);
    // push level in level list
    levelList->push_back(curLevel);

    // Add map for original nodes - use for set link in replics to original node
    std::map <unsigned, ProjectionElem*> origianlNodes;
    origianlNodes.insert({nodeId, elem});
    // Remove perspectived node - already in projection
    graphNodes->erase(nodeId);

    ProjectionLevelElem* parentLevel;
    unsigned projectionLevel = 0;
    bool isComlpete = false;

    while (!isComlpete)
    {
        parentLevel = levelList->at(projectionLevel);
        curLevel = new ProjectionLevelElem();
        bool isEmpty = true;

        for (const auto &parentIt : *parentLevel)
        {
            ProjectionElem* parent = parentIt;
            if (!parent->isOriginal())
                continue;

            isEmpty = false;

            const Node* node = graph.getNode(parentIt->getId());
            const EdgeList* edgeList = node->getEdges();
            for (const auto &edgeIt : *edgeList)
            {
                unsigned curId = edgeIt.first;
                // skip node if already it's parent in path
                if (parent->findInParents(curId))
                    continue;

                // insert elem as child in parent elem
                elem = parent->addElem(curId);
                elem->setLevel(projectionLevel);
                // insert elem in current projection level list
                curLevel->push_back(elem);

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
            // Sort level for fast search
            std::sort(curLevel->begin(), curLevel->end(), ProjectionElem::less);
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
    updateShortestLoop();
}



void Projection::createLastLevelProjection(const GraphBase& graph)
{
    // Last level find original elements on last level
    // which have edges between together
    std::deque <ProjectionElem*> nodes;
    ProjectionLevelElem* parentLevel = levelList->back();
    for (const auto &elem : *parentLevel)
    {
        if (elem->isOriginal())
        {
            nodes.push_back(elem);
        }
    }
    // If more one original nodes in last level
    // need to add new level for add edge info between together
    if (nodes.size() > 1)
    {
        unsigned size = nodes.size();
        ProjectionLevelElem* curLevel = new ProjectionLevelElem;
        for(unsigned i = 0; i < size - 1; ++i)
        {
            ProjectionElem* elem = nodes.at(i);
            for (unsigned j = i + 1; j < size; ++j)
            {
               ProjectionElem* elem2 = nodes.at(j);
               if ( graph.getEdge(elem->getId(), elem2->getId()) )
               {
                   // Add second elem in first
                   unsigned curId = elem2->getId();
                   // insert elem as child in parent elem
                   ProjectionElem* el = elem->addElem(curId);
                   el->setOriginal(elem2);
                   // insert elem in current projection level list
                   curLevel->push_back(el);

                   // Add first elem in second
                   curId = elem->getId();
                   // insert elem as child in parent elem
                   el = elem2->addElem(curId);
                   el->setOriginal(elem);
                   // insert elem in current projection level list
                   curLevel->push_back(el);
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
    std::map <unsigned, ProjectionElem*> origianlNodes;

    for(const auto &level : *levelList)
    {
        for (const auto &it : *level)
        {
            auto res = origianlNodes.insert({it->getId(), it});
            if (!res.second)
            {
                // set node as replica - set original node address
                it->setOriginal(res.first->second);
            }
        }
    }
}



void Projection::updateEccesntricity()
{
    if (!levelList)
        return;

    ProjectionLevelElem* level = levelList->back();
    for (const auto &elem : *level)
    {
        // If one node is original so all original nodes on this level
        // have no edges between together
        if (elem->isOriginal())
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



/**
 * @brief Projection::updateShortestLoop find in projection shortest loop
 * and update shortestLoop member
 */
void Projection::updateShortestLoop()
{
    if (!levelList || !levelList->size())
        return;

    shortestLoop = UINT32_MAX;
    unsigned origId;
    unsigned endL = levelList->size();

    for(unsigned origN = 1; origN < endL; ++origN)
    {
        // if start for this we not find less.
        if (2 * origN >= shortestLoop)
            break;

        const auto &origL = *levelList->at(origN);
        // move by level find original node
        for (auto origIt = origL.begin(), origEnd = origL.end();
             origIt != origEnd; ++origIt)
        {
            if (!(*origIt)->isOriginal())
            {
                continue;
            }

            origId = (*origIt)->getId();
            bool find = false;
            unsigned replN = origN;
            // Skip first level for search - only original nodes
            if (origN > 1)
            {
                // find replica of this node in current level
                auto replIt = origIt;
                for (++replIt; replIt != origEnd; ++replIt)
                {
                    if ((*replIt)->getId() == origId &&
                        !(*replIt)->isOriginal())
                    {
                        find = true;
                        break;
                    }
                }
            }

            if (!find)
            {
                for(++replN; replN != endL; ++replN)
                {
                    const auto &replL = *levelList->at(replN);
                    const auto replEnd = replL.end();
                    // find replica of this node in next levels
                    for (auto replIt = replL.begin(); replIt != replEnd;
                         ++replIt)
                    {
                        if ((*replIt)->getId() == origId &&
                            !(*replIt)->isOriginal())
                        {
                            find = true;
                            break;
                        }
                    }
                    if (find)
                        break;
                }
            }

            // For this orig node not exist replica
            if (!find)
                continue;

            unsigned currentN = origN + replN;
            // minimal loop accessed. less not exist.
            if (currentN == 3)
            {
                shortestLoop = currentN;
                return;
            }
            if (currentN < shortestLoop)
            {
                shortestLoop = currentN;
            }

            // In this place check if replica find in same level origN == replN
            // so search to deeper have no reason
            if (2 * origN >= shortestLoop)
                return;
        }
    }
}



/**
 * @brief Projection::getProjectionNodeStat returns vector of projection
 * level nodes statistic. Vector contains numbers pairs.
 * First number contain original count, second contain replica count.
 * Level size is half part size vector.
 * @return vector of node numbers size of doubled levels count.
 */
uint_vec* Projection::getProjectionNodeStat() const
{
    if (!levelList->size())
        return nullptr;

    unsigned i = 0;
    uint_vec* list = new uint_vec(levelList->size() * 2, 0);
    for (const auto &level : *levelList)
    {
        for (const auto &it : *level)
        {
            if (it->isOriginal())
                ++((*list)[i]);
            else
                ++((*list)[i + 1]);
        }
        i += 2;
    }
    return list;
}



void Projection::sortAllProjections()
{
    if (!levelList || !levelList->size())
        return;

    auto level = levelList->begin();
    auto end = levelList->end();
    for (++level; level < end; ++level)
    {
        std::sort((*level)->begin(), (*level)->end(), ProjectionElem::less);
    }
}



void Projection::updateAfterRead()
{
    updateOriginalInfo();
    sortAllProjections();
    updateEccesntricity();
    updateShortestLoop();
}
