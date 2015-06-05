#ifndef PROJECTION_H
#define PROJECTION_H


#include <cstddef>
#include <map>
#include <deque>
#include <vector>

#include "types.h"

#include "ProjectionsReader.h"

class GraphBase;
class ProjectionElem;



typedef std::deque <ProjectionElem*> ProjectionLevelElem;
typedef std::deque <ProjectionLevelElem*> ProjectionLevelList;
typedef std::vector <std::vector<unsigned>*> ProjShortPaths;


class Projection
{
public:
    friend class ProjectionsReader;

    Projection(unsigned nodeId);
    ~Projection();

    static bool less(const Projection* a, const Projection* b);
    static bool lessById(const Projection* a, const unsigned &id);


    void clear();

    bool isEmpty() const;
    bool fileExist() const;

    unsigned getId() const;
    unsigned levelCount() const;
    unsigned getEccentricity() const;
    unsigned getShortestLoop() const;

    const ProjectionElem* getRootNode() const;
    uint_vec* getProjectionNodeStat() const;

    ProjShortPaths* findShortPaths(unsigned nodeId, bool reverse = false);

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
    bool isFileExist;

    void createLastLevelProjection(const GraphBase& graph);
    void sortAllProjections();

    void updateAfterRead();

private:
    void clearLevels();
};

#endif // PROJECTION_H
