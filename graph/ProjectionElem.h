#ifndef PROJECTIONELEM_H
#define PROJECTIONELEM_H

#include <cstddef>
#include <map>

class ProjectionElem;

typedef std::map <unsigned, ProjectionElem*> ProjectionElemMap;
typedef std::pair<ProjectionElemMap::const_iterator,
                  ProjectionElemMap::const_iterator> ProjectionElemMapItPair;


class ProjectionElem
{
public:
    ProjectionElem(unsigned nodeId);
    ~ProjectionElem();

    unsigned getId() const;
    unsigned listCount() const;

    bool isOriginal() const;
    bool isLeaf() const;
    bool isEmpty() const;

    ProjectionElem* getParent() const;
    void setParent(ProjectionElem* newParent);
    void setOriginal(ProjectionElem* newOriginal);

    const ProjectionElem* findInParents(unsigned searchId) const;

    const ProjectionElemMap* getList() const;

    ProjectionElem* addElem(unsigned nodeId);
    void eraseElem(const ProjectionElemMap::const_iterator pos);

protected:
    unsigned nodeId;
    ProjectionElem* parent;
    ProjectionElem* originalElem;
    ProjectionElemMap* listElem;

};

#endif // PROJECTIONELEM_H
