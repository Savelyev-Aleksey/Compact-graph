#include "ProjectionElem.h"



ProjectionElem::ProjectionElem(size_t nodeId) :
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



size_t ProjectionElem::getId() const
{
    return nodeId;
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



const ProjectionElemMap* ProjectionElem::getList() const
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



void ProjectionElem::addElem(ProjectionElem *child)
{
    size_t id = child->getId();
    listElem->insert({id, child});
}



void ProjectionElem::eraseElem(const ProjectionElemMap::const_iterator pos)
{
    listElem->erase(pos);
}



/**
 * @brief ProjectionElem::isNodeParent - Check that searchId not yet inserted as
 * parent in tree lower projections.
 * @param elem - current elem there searchId node will be inserted
 * @param searchId - node id
 * @return node pointer if node already in lower projections
 */
const ProjectionElem* ProjectionElem::isNodeParent(size_t searchId) const
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


