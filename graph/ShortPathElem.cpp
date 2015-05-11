
#include <cstddef>

#include "ShortPathElem.h"



size_t ShortPathElem::count = 0;



size_t ShortPathElem::getCount()
{
    return ShortPathElem::count;
}



ShortPathElem::ShortPathElem(float weight, size_t indent) :
    list(nullptr),
    weight(weight),
    indent(indent)
{
    // list lazy init
    ++count;
}



ShortPathElem::~ShortPathElem()
{
    delete list;
    --count;
}



void ShortPathElem::setWeight(float newWeight)
{
    weight = newWeight;
}



float ShortPathElem::getWeight() const
{
    return weight;
}



void ShortPathElem::setIndent(size_t newIndent)
{
    indent = newIndent;
}


size_t ShortPathElem::getIndent() const
{
    return indent;
}



/**
 * @brief ShortPathElem::addNodeElem - Add elem into node list current elem
 * @param nodeNum - Inserting node id
 * @param weight - Total weight from root node to current elem
 * @return true if inserted, false if already was inserted
 */
ShortPathElem* ShortPathElem::addNodeElem(size_t nodeNum, float weight,
                                          size_t indent)
{
    // Lazy init
    if (!list)
    {
        list = new PathList;
    }
    ShortPathElem* elem = new ShortPathElem(weight, indent);
    bool result = list->insert(
                std::pair<size_t, ShortPathElem* > (nodeNum, elem) ).second;
    if (result)
    {
        return elem;
    }
    delete elem;
    return nullptr;
}



/**
 * @brief ShortPathElem::addNodeElem - Add elem into node list current elem
 * @param nodeNum - Inserting node id
 * @param pathElem - pointer on hierarchy placed node parent in which can seek
 * this elem
 */
void ShortPathElem::addNodeElem(size_t nodeNum, ShortPathElem *pathElem)
{
    if (!list)
    {
        list = new PathList;
    }
    list->insert(std::pair<size_t, ShortPathElem*> (nodeNum, pathElem) );
}



/**
 * @brief ShortPathElem::popNodeElem - Remove elem from node list.
 * @param nodeNum - Removing node id
 * @return true if removed, false otherwise
 */
bool ShortPathElem::popNodeElem(size_t nodeNum)
{
    if (!list)
    {
        return false;
    }
    auto it = list->find(nodeNum);
    if (it == list->end())
    {
        return false;
    }
    list->erase(it);
    return true;
}



/**
 * @brief ShortPathElem::findElem - Find node by nodeNum in current list
 * @param nodeNum - searched nodeElem id
 * @return pointer on ShortPathElem or null if not found
 */
ShortPathElem* ShortPathElem::findElem(size_t nodeNum) const
{
    if (!list)
    {
        return nullptr;
    }
    auto it = list->find(nodeNum);
    if (it == list->end())
    {
        return nullptr;
    }
    return it->second;
}



/**
 * @brief ShortPathElem::updateParent - Change parent pointer for seek map if
 * node placed to new hierarchy node
 * @param nodeNum - node id
 * @param parent - parent of node with nodeNum id
 */
void ShortPathElem::updateParent(size_t nodeNum, ShortPathElem* parent)
{
    if (!list)
    {
        return;
    }
    auto it = list->find(nodeNum);
    if (it == list->end())
    {
        return;
    }
    it->second = parent;
}



PathList* ShortPathElem::getPathList() const
{
    return list;
}

