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

typedef std::map <unsigned, Node*> NodeMap;
typedef std::set <unsigned> NodeIdSet;
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

    unsigned nodeCount() const;
    unsigned edgeCount() const;

          Node* getNodeOrCreate(unsigned nodeNum);
    Node* getNode(unsigned nodeNum) const;
    Edge* getEdge(unsigned nodeFromNum, unsigned nodeToNum) const;
    NodeMap* getNodeMap() const;
    NodeIdSet* getNodeIds();

    UintMap* getNodeDegreeStatistic() const;

    bool addEdge(unsigned nodeFromNum, unsigned nodeToNum, float value);
    bool addEdge(Node* nodeFrom, Node* nodeTo, float value);
    bool addNode(unsigned nodeNum);
    Node *newNode(unsigned nodeNum);

    void addInfo(const char *str);
    void addInfo(const char *name, const char* value);
    const char* getInfo(const char *name) const;
    const InfoDeque* getAllInfo() const;
    void printInfo(FILE* f) const;
    void scanInfo(FILE* f);

protected:
    NodeMap* nodeList;
    InfoDeque* info;

    unsigned edgeSize;

    void clearInfo();
    void clearNodes();
};

#endif	/* GRAPHBASE_H */

