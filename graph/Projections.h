#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <cstddef>
#include <deque>

#include "types.h"
#include "Worker.h"

#include "ProjectionsReader.h"

class Projection;
class GraphBase;

typedef std::deque <Projection*> ProjectionsList;


class Projections : public Worker
{
public:
    Projections(GraphBase& graph);
    virtual ~Projections();

    friend bool ProjectionsReader::readProjections(FILE *fp,
                                                   FileTypes::Type typeId);

    bool isEmpty() const;

    void clear();

    unsigned size() const;

    const ProjectionsList* getList() const;
    GraphBase& getGraph() const;

    Projection* getProjection(unsigned nodeId) const;
    UintMap* getEccentriciyStatistic() const;
    unsigned getGraphGirth() const;

    void createAllProjections();
    void createProjection(unsigned nodeId);

    bool isProjectionExist(unsigned nodeId) const;

protected:
    GraphBase* graph;
    ProjectionsList* projectionsList;
};

#endif // PROJECTIONS_H
