#ifndef SHORTPATH_H
#define SHORTPATH_H

#include <cstddef>
#include <map>
#include <deque>
#include <forward_list>
#include "types.h"

class ShortPathElem;
class ShortPathRootElem;
class GraphBase;
class Node;

// first hierarchy struct, second basic map
typedef std::map  <size_t, ShortPathRootElem*> RootPathList;
typedef std::deque <size_t> NodeIdDeque;
typedef std::deque <Node *> NodeDeque;



class ShortPath
{
public:
    ShortPath(GraphBase& graph);
    ~ShortPath();

    void generateAllShortPaths(float pathLimit = 0);
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
    void createPaths(NodeDeque& nodesToVisit, ShortPathRootElem* rootElem,
                     float pathLimit);
    void updateParams(ShortPathElem* elem, float difference, size_t indent);

};

#endif // SHORTPATH_H
