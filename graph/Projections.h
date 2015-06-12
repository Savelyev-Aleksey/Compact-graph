#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <cstddef>
#include <deque>

#include "types.h"
#include "Worker.h"

#include "Projection.h"
#include "ProjectionsReader.h"

class GraphBase;

typedef std::deque <Projection*> ProjectionsList;


class Projections : public Worker
{
public:
    Projections(GraphBase& graph);
    virtual ~Projections();

    friend bool ProjectionsReader::
                readProjections(FILE *fp, ProjectionsReader::Type typeId);

    bool isEmpty() const;

    virtual void clear();

    unsigned size() const;

    const ProjectionsList* getList() const;
    GraphBase& getGraph() const;

    UintMap* getEccentriciyStatistic() const;
    unsigned getGraphGirth() const;

    virtual bool isProjectionExist(unsigned nodeId) const;
    virtual Projection* getProjection(unsigned nodeId) const;

    virtual void createAllProjections();
    virtual Projection* createProjection(unsigned nodeId);

    virtual ProjShortPaths* findShortPaths(unsigned fromId, unsigned toId,
                                           bool reverse = false);

protected:
    GraphBase* graph;
    ProjectionsList* projectionsList;
};

#endif // PROJECTIONS_H
