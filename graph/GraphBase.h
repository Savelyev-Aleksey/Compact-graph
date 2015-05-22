#ifndef GRAPHBASE_H
#define	GRAPHBASE_H

#include <cstddef>
#include <cstdio>
#include <deque>
#include <map>
#include <set>

#include "types.h"


class Node;
class Edge;

typedef std::map <size_t, Node*> NodeMap;
typedef std::set <size_t> NodeIdSet;
typedef std::deque < std::pair<char*, char*> > InfoDeque;


/**
 * @brief The GraphBase class - This is basic class to operate simple
 * graph structure. It's contain Node elements, organized by binary tree by
 * std::map structure.
 * Each Node element contain std::map structure of Edges, out from node.
 * Key in map contain node id where edge pointed.
 */
class GraphBase {
public:
    GraphBase();
    ~GraphBase();

    void clear();

    bool isEmpty() const;

    size_t size() const;

          Node* getNodeOrCreate(size_t nodeNum);
    const Node* getNode(size_t nodeNum) const;
    const Edge* getEdge(size_t nodeFromNum, size_t nodeToNum) const;
    NodeMap* getNodeMap() const;
    NodeIdSet* getNodeIds();

    UlongMap* getNodeDegreeStatistic() const;

    bool addEdge(size_t nodeFromNum, size_t nodeToNum, float value);
    bool addEdge(Node* nodeFrom, Node* nodeTo, float value);
    bool addNode(size_t nodeNum);
    Node *newNode(size_t nodeNum);

    void addInfo(const char *str);
    void addInfo(const char *name, const char* value);
    const char* getInfo(const char *name) const;
    const InfoDeque* getAllInfo() const;
    void printInfo(FILE* f) const;
    void scanInfo(FILE* f);

    void logStatus();

protected:
    NodeMap* nodeList;
    InfoDeque* info;

    void clearInfo();
    void clearNodes();
};

#endif	/* GRAPHBASE_H */

