#ifndef PROJECTION_H
#define PROJECTION_H


#include <cstddef>
#include <map>
#include <deque>

#include "types.h"

#include "ProjectionsReader.h"

class GraphBase;
class ProjectionElem;



typedef std::multimap <unsigned, ProjectionElem*> ProjectionLevelElem;
typedef std::deque <ProjectionLevelElem*> ProjectionLevelList;


class Projection
{
public:
    Projection(unsigned nodeId);
    ~Projection();

    friend bool ProjectionsReader::readProjections(FILE* fp,
                                                   FileTypes::Type typeId);

    void clear();

    unsigned getId() const;
    unsigned levelCount() const;
    unsigned getEccentricity() const;
    unsigned getShortestLoop() const;

    const ProjectionElem* getRootNode() const;
    uint_vec* getProjectionNodeStat() const;

    void createProjection(GraphBase& graph);

    void updateOriginalInfo();
    void updateEccesntricity();
    void updateShortestLoop();

protected:
    unsigned nodeId;
    ProjectionElem* rootNode;
    ProjectionLevelList* levelList;

    unsigned eccesntricity;
    unsigned shortestLoop;

    void createLastLevelProjection(const GraphBase& graph);

    void updateAfterRead();

};

#endif // PROJECTION_H
