#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <cstddef>
#include <map>

#include "Worker.h"

class Projection;
class GraphBase;

typedef std::map <size_t, Projection*> ProjectionsList;


class Projections : public Worker
{
public:
    Projections(GraphBase& graph);
    ~Projections();

    void createAllProjections();
    void createProjection(size_t nodeId);

protected:
    GraphBase* graph;
    ProjectionsList* projections;
};

#endif // PROJECTIONS_H
