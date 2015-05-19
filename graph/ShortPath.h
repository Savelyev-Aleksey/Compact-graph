#ifndef SHORTPATH_H
#define SHORTPATH_H

#include <cstddef>
#include <map>
#include <deque>
#include <forward_list>

#include "types.h"
#include "Worker.h"

class ShortPathElem;
class ShortPathRootElem;
class GraphBase;
class Node;

typedef std::pair <const size_t, ShortPathElem*> PathPair;
// first hierarchy struct, second basic map
typedef std::map  <size_t, ShortPathRootElem*> RootPathList;
typedef std::deque <size_t> NodeIdDeque;
typedef std::deque <const Node *> NodeDeque;



class ShortPath: public Worker
{
public:
    ShortPath(GraphBase& graph);
    ~ShortPath();

    void generateAllShortPaths(float pathLimit);
    void generateShortPath(const size_t startNodeId, float pathLimit = 0);
    bool isPathExist(size_t nodeId) const;

    GraphBase& getGraph() const;
    RootPathList* getShortPathNodes();

    size_t getShortPathCount() const;

    UlongMap* getEccentriciyStatistic() const;

    void clearPath(size_t nodeId);
    void clearAllPaths();

    bool isEmpty() const;

    ShortPathRootElem* initShortPath(size_t startNodeId);

protected:
    GraphBase* graph;
    RootPathList *shortPathNodes;


    bool isNodeForVisit(const NodeDeque& list, size_t nodeId) const;
    void createPath(NodeDeque& nodesToVisit, ShortPathRootElem* rootElem,
                    float pathLimit);
    void updateSubpath(PathPair* pair, float difference, size_t indent,
                       NodeDeque& nodesToVisit);

};

#endif // SHORTPATH_H
