#ifndef NODE_H
#define	NODE_H

#include <cstddef>
#include <map>
#include <deque>

class Edge;

typedef std::map<unsigned, Edge *> EdgeList;
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
    Node(unsigned id);
    ~Node();

    unsigned  getId() const;
    void setId(unsigned num);
    bool addEdge(Edge* edge);
    const Edge* getEdge(unsigned nodeToNum) const;
    const EdgeList* getEdges() const;
    size_t getEdgeCount() const;
    void deleteEdgePointer(unsigned nodeToNum);

    WeightedEdges* getValueSortedEdges();

protected:
    unsigned nodeId;
    /**
     * @brief edgeList - map contains list of edges
     * by pair <toNodeNum, Edge pointer>
     */
    EdgeList *edgeList;

};

#endif	/* NODE_H */

