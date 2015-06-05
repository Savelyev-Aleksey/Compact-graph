#ifndef PROJECTIONELEM_H
#define PROJECTIONELEM_H

#include <cstddef>
#include <deque>

class ProjectionElem;

typedef std::deque <ProjectionElem*> ProjectionElemList;
typedef std::pair<ProjectionElemList::const_iterator,
                  ProjectionElemList::const_iterator> ProjectionElemListItPair;


class ProjectionElem
{
public:
    ProjectionElem(unsigned nodeId);
    ~ProjectionElem();

    static bool less(const ProjectionElem* a, const ProjectionElem* b);
    static bool lessPrId(const ProjectionElem* a, const unsigned& id);
    static bool lessIdPr(const unsigned& id, const ProjectionElem* a);

    unsigned getId() const;
    unsigned listCount() const;
    unsigned getLevel() const;

    bool isOriginal() const;
    bool isLeaf() const;
    bool isEmpty() const;

    ProjectionElem* getParent() const;
    void setParent(ProjectionElem* newParent);
    void setOriginal(ProjectionElem* newOriginal);
    void setLevel(unsigned newLevel);

    const ProjectionElem* findInParents(unsigned searchId) const;

    const ProjectionElemList* getList() const;

    ProjectionElem* addElem(unsigned nodeId);
    void eraseElem(const ProjectionElemList::const_iterator pos);

protected:
    unsigned nodeId;
    unsigned level;
    ProjectionElem* parent;
    ProjectionElem* originalElem;
    ProjectionElemList* listElem;

};

#endif // PROJECTIONELEM_H
