
#include <cstddef>

#include "ShortPathElem.h"



ShortPathElem::ShortPathElem(unsigned nodeId, float weight, unsigned indent) :
    parent(nullptr),
    list(nullptr),
    nodeId(nodeId),
    weight(weight),
    indent(indent)
{}



ShortPathElem::~ShortPathElem()
{
    delete list;
}



unsigned ShortPathElem::size() const
{
    if (!list)
        return 0u;
    return list->size();
}



void ShortPathElem::setWeight(float newWeight)
{
    weight = newWeight;
}



float ShortPathElem::getWeight() const
{
    return weight;
}



void ShortPathElem::setIndent(unsigned newIndent)
{
    indent = newIndent;
}


unsigned ShortPathElem::getIndent() const
{
    return indent;
}



void ShortPathElem::setParent(ShortPathElem *newParent)
{
    parent = newParent;
}



ShortPathElem* ShortPathElem::getParent() const
{
    return parent;
}



/**
 * @brief ShortPathElem::addNodeElem - Add elem into node list current elem
 * @param nodeNum - Inserting node id
 * @param weight - Total weight from root node to current elem
 * @return true if inserted, false if already was inserted
 */
ShortPathElem* ShortPathElem::addNodeElem(unsigned nodeNum, float weight,
                                          unsigned indent)
{
    // Lazy init
    if (!list)
    {
        list = new PathList;
    }
    ShortPathElem* elem = new ShortPathElem(nodeNum, weight, indent);
    elem->setParent(this);
    bool result = list->insert({nodeNum, elem}).second;
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
void ShortPathElem::addNodeElem(unsigned nodeNum, ShortPathElem *pathElem)
{
    if (!list)
    {
        list = new PathList;
    }
    list->insert({nodeNum, pathElem});
}



/**
 * @brief ShortPathElem::popNodeElem - Remove elem from node list.
 * @param nodeNum - Removing node id
 * @return true if removed, false otherwise
 */
bool ShortPathElem::popNodeElem(unsigned nodeNum)
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
ShortPathElem* ShortPathElem::findElem(unsigned nodeNum) const
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



PathList* ShortPathElem::getPathList() const
{
    return list;
}

