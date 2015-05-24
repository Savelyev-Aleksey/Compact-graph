#ifndef PROJECTIONELEM_H
#define PROJECTIONELEM_H

#include <cstddef>
#include <map>

class ProjectionElem;

typedef std::map <size_t, ProjectionElem*> ProjectionElemMap;


class ProjectionElem
{
public:
    ProjectionElem(size_t nodeId);
    ~ProjectionElem();

    size_t getId() const;

    bool isOriginal() const;
    bool isLeaf() const;

    ProjectionElem* getParent() const;
    void setParent(ProjectionElem* newParent);
    void setOriginal(ProjectionElem* newOriginal);

    const ProjectionElem* isNodeParent(size_t searchId) const;

    const ProjectionElemMap* getList() const;

    void addElem(ProjectionElem* child);
    void eraseElem(const ProjectionElemMap::const_iterator pos);

protected:
    size_t nodeId;
    ProjectionElem* parent;
    ProjectionElem* originalElem;
    ProjectionElemMap* listElem;

};

#endif // PROJECTIONELEM_H
