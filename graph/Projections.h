#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <cstddef>
#include <map>

#include "types.h"
#include "Worker.h"

class Projection;
class GraphBase;

typedef std::map <size_t, Projection*> ProjectionsList;


class Projections : public Worker
{
public:
    Projections(GraphBase& graph);
    virtual ~Projections();

    bool isEmpty() const;

    void clear();

    size_t size() const;

    const ProjectionsList* getList() const;

    const Projection* getProjection(size_t nodeId) const;
    UlongMap* getEccentriciyStatistic() const;

    void createAllProjections();
    void createProjection(size_t nodeId);

    bool isProjectionExist(size_t nodeId) const;

protected:
    GraphBase* graph;
    ProjectionsList* projections;
};

#endif // PROJECTIONS_H
