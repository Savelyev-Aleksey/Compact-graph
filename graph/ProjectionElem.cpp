#include "ProjectionElem.h"



ProjectionElem::ProjectionElem(unsigned nodeId) :
    nodeId(nodeId),
    parent(nullptr),
    originalElem(nullptr),
    listElem(nullptr)
{}



ProjectionElem::~ProjectionElem()
{
    if (listElem)
        delete listElem;
}



unsigned ProjectionElem::getId() const
{
    return nodeId;
}



unsigned ProjectionElem::listCount() const
{
    if (!listElem)
        return 0;
    return listElem->size();
}



ProjectionElem* ProjectionElem::getParent() const
{
    return parent;
}



void ProjectionElem::setParent(ProjectionElem *newParent)
{
    parent = newParent;
}



void ProjectionElem::setOriginal(ProjectionElem *newOriginal)
{
    originalElem = newOriginal;
}



void ProjectionElem::setLevel(unsigned newLevel)
{
    level = newLevel;
}



unsigned ProjectionElem::getLevel() const
{
    return level;
}



const ProjectionElemList* ProjectionElem::getList() const
{
    return listElem;
}



bool ProjectionElem::isOriginal() const
{
    return originalElem == nullptr;
}



bool ProjectionElem::isLeaf() const
{
    return listElem ? listElem->size() == 0 : true;
}



bool ProjectionElem::isEmpty() const
{
    return isLeaf();
}



ProjectionElem* ProjectionElem::addElem(unsigned nodeId)
{
    if (!listElem)
        listElem = new ProjectionElemList;

    ProjectionElem* el = new ProjectionElem(nodeId);
    el->setParent(this);
    listElem->push_back(el);
    return el;
}



void ProjectionElem::eraseElem(const ProjectionElemList::const_iterator pos)
{
    if (listElem)
        listElem->erase(pos);
}



/**
 * @brief ProjectionElem::findInParents - Find searchId  node as
 * parent in tree lower projections.
 * @param elem - current elem there searchId node will be inserted
 * @param searchId - node id
 * @return node pointer if node already in lower projections
 * else nullptr.
 */
const ProjectionElem* ProjectionElem::findInParents(unsigned searchId) const
{
    const ProjectionElem* el = this->parent;
    while (el)
    {
        if (el->getId() == searchId)
            return el;
        el = el->getParent();
    }
    return nullptr;
}



bool ProjectionElem::less(const ProjectionElem *a, const ProjectionElem *b)
{
    return (a->nodeId <  b->nodeId) ||
           (a->nodeId == b->nodeId  && !a->originalElem);
}



bool ProjectionElem::lessById(const ProjectionElem *a, const unsigned id)
{
    return a->nodeId < id;
}
