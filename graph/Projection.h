#ifndef PROJECTION_H
#define PROJECTION_H


#include <cstddef>
#include <map>
#include <deque>


class GraphBase;
class ProjectionElem;


typedef std::multimap <size_t, ProjectionElem*> ProjectionLevelElem;
typedef std::deque <ProjectionLevelElem*> ProjectionLevelList;


class Projection
{
public:
    Projection(size_t nodeId);
    ~Projection();

    size_t getId() const;

    void createProjection(GraphBase& graph);

protected:
    size_t nodeId;
    ProjectionElem* rootNode;
    ProjectionLevelList* levelList;

    size_t eccesntricity;

};

#endif // PROJECTION_H