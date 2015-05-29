#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <cstddef>
#include <map>

#include "types.h"
#include "Worker.h"

#include "ProjectionsReader.h"

class Projection;
class GraphBase;

typedef std::map <size_t, Projection*> ProjectionsList;


class Projections : public Worker
{
public:
    Projections(GraphBase& graph);
    virtual ~Projections();

    friend bool ProjectionsReader::readProjections(FILE *fp,
                                                   FileTypes::Type typeId);

    bool isEmpty() const;

    void clear();

    size_t size() const;

    const ProjectionsList* getList() const;
    GraphBase& getGraph() const;

    Projection* getProjection(size_t nodeId) const;
    UlongMap* getEccentriciyStatistic() const;
    size_t getGraphGirth() const;

    void createAllProjections();
    void createProjection(size_t nodeId);

    bool isProjectionExist(size_t nodeId) const;

protected:
    GraphBase* graph;
    ProjectionsList* projectionsList;
};

#endif // PROJECTIONS_H
