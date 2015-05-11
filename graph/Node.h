#ifndef NODE_H
#define	NODE_H

#include <cstddef>
#include <map>
#include <deque>

class Edge;

typedef std::map<size_t, Edge *> EdgeList;
typedef std::pair<EdgeList::const_iterator, EdgeList::const_iterator>
        EdgeListPairIt;
typedef std::deque < std::pair<float, Edge*> > WeightedEdges;

/**
 * @brief The Node class - This is basic class of Node.
 * It's contain node id - unique number of node.
 * It's contain edge list - std::map od pairs:
 * node id where pointed edge and edge pointer.
 */
class Node {
public:
    Node(size_t id);
    ~Node();

    size_t  getId() const;
    void setId(size_t num);
    bool addEdge(Edge* edge);
    const Edge* getEdge(size_t nodeToNum) const;
    const EdgeList* getEdges() const;
    size_t getEdgeCount() const;
    void deleteEdgePointer(size_t nodeToNum);

    WeightedEdges* getValueSortedEdges();

    static size_t getCount();
private:
    static size_t count;
    size_t nodeId;
    /**
     * @brief edgeList - map contains list of edges
     * by pair <toNodeNum, Edge pointer>
     */
    EdgeList *edgeList;

};

#endif	/* NODE_H */

