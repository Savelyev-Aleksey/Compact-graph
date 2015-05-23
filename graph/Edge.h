#ifndef EDGE_H
#define	EDGE_H

#include <cstddef>

class Node;



/**
 * @brief The Edge class. This is basic egde class.
 * It's contain two poniter on two edges.
 * It's contain float value - weight of egde.
 */
//template <typename _Value>
class Edge
{
public:
    Edge(Node* node1, Node* node2, float value);
    ~Edge();

    float getValue();
    void setValue(float);

    Node* nodeElem(size_t nodeNum);
    Node* nodeElem1();
    Node* nodeElem2();

    size_t nodeToNum(size_t nodeFromNum);
    Node* nodeTo(size_t nodeFromNum);

    void deleteEdge();
    void deleteEdge(size_t nodeFromNum);

protected:
    Node* node1;
    Node* node2;
    float value;

};

#endif	/* EDGE_H */

