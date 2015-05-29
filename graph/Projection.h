#ifndef PROJECTION_H
#define PROJECTION_H


#include <cstddef>
#include <map>
#include <deque>

#include "types.h"

#include "ProjectionsReader.h"

class GraphBase;
class ProjectionElem;



typedef std::multimap <size_t, ProjectionElem*> ProjectionLevelElem;
typedef std::deque <ProjectionLevelElem*> ProjectionLevelList;


class Projection
{
public:
    Projection(size_t nodeId);
    ~Projection();

    friend bool ProjectionsReader::readProjections(FILE* fp,
                                                   FileTypes::Type typeId);

    void clear();

    size_t getId() const;
    size_t levelCount() const;
    size_t getEccentricity() const;
    size_t getShortestLoop() const;

    const ProjectionElem* getRootNode() const;
    size_vec* getProjectionNodeStat() const;

    void createProjection(GraphBase& graph);

    void updateOriginalInfo();
    void updateEccesntricity();
    void updateShortestLoop();

protected:
    size_t nodeId;
    ProjectionElem* rootNode;
    ProjectionLevelList* levelList;

    size_t eccesntricity;
    size_t shortestLoop;

    void createLastLevelProjection(const GraphBase& graph);

    void updateAfterRead();

};

#endif // PROJECTION_H
