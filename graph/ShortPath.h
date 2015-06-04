#ifndef SHORTPATH_H
#define SHORTPATH_H

#include <cstddef>
#include <map>
#include <deque>

#include "types.h"
#include "Worker.h"

class ShortPathElem;
class ShortPathRootElem;
class GraphBase;
class Node;

typedef std::pair <cuint, ShortPathElem*> PathPair;
// TODO: move paths on deque instead map, add less comp func and others
typedef std::map  <unsigned, ShortPathRootElem*> RootPathList;
typedef std::deque <unsigned> NodeIdDeque;
typedef std::deque <const Node *> NodeDeque;



class ShortPath: public Worker
{
public:
    ShortPath(GraphBase& graph);
    ~ShortPath();

    void createAllShortPaths(float pathLimit);
    void generateShortPath(const unsigned startNodeId, float pathLimit = 0);
    bool isPathExist(unsigned nodeId) const;

    GraphBase& getGraph() const;
    RootPathList* getShortPathNodes();

    unsigned getShortPathCount() const;

    UintMap* getEccentriciyStatistic() const;

    void clearPath(unsigned nodeId);
    void clear();

    bool isEmpty() const;

    ShortPathRootElem* initShortPath(unsigned startNodeId);
    void initShortAllPaths();

protected:
    GraphBase* graph;
    RootPathList *shortPathNodes;


    bool isNodeForVisit(const NodeDeque& list, unsigned nodeId) const;
    void createPath(NodeDeque& nodesToVisit, ShortPathRootElem* rootElem,
                    float pathLimit);
    void updateSubpath(PathPair* pair, float difference, unsigned indent,
                       NodeDeque& nodesToVisit);

private:
    void clearPathList();
};

#endif // SHORTPATH_H
