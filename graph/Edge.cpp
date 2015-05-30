
#include <cstdlib>

#include "Edge.h"
#include "Node.h"



Edge::Edge(Node* node1, Node* node2, float value) :
    node1(node1),
    node2(node2),
    value(value)
{}



Edge::~Edge()
{}



float Edge::getValue()
{
    return value;
}



void Edge::setValue(float value)
{
    if (value > 0)
        this->value = value;
}



/**
 * @brief Edge::nodeElem - Get node pointer by node id
 * @param nodeNum - node id
 * @return node pointer
 */
Node* Edge::nodeElem(unsigned nodeNum)
{
    if (nodeNum == node1->getId())
    {
        return node1;
    }
    else if (nodeNum == node2->getId())
    {
        return node2;
    }
    return nullptr;
}



/**
 * Return pointer on first node
 * @return - node pointer
 */
Node* Edge::nodeElem1()
{
    return node1;
}



/**
 * Return pointer on second node
 * @return - node pointer
 */
Node* Edge::nodeElem2()
{
    return node2;
}



/**
 * @brief Edge::node - Returns another node num then looks from one node.
 * @param fromNodeNum - Node id from which looks
 * @return node id
 */
unsigned Edge::nodeToNum(unsigned nodeFromNum)
{
    return node1->getId() == nodeFromNum ? node2->getId() : node1->getId();
}



/**
 * @brief Edge::node - Returns another node  then looks from one node.
 * @param fromNodeNum - Node id from which looks
 * @return node pointer
 */
Node* Edge::nodeTo(unsigned nodeFromNum)
{
    return node1->getId() == nodeFromNum ? node2 : node1;
}



/**
 * @brief Edge::deleteEdge - destructor remove pointer from both nodes
 */
void Edge::deleteEdge()
{
    size_t n1 = node1->getId();
    size_t n2 = node2->getId();
    node1->deleteEdgePointer(n2);
    node2->deleteEdgePointer(n1);
    delete this;
}



/**
 * @brief Edge::deleteEdge - destructor remove pointer from second node where
 * removing first node
 * @param nodeFromNum - first node num
 */
void Edge::deleteEdge(unsigned nodeFromNum)
{
    Node* nodeTo = nodeFromNum == node1->getId() ? node2 : node1;
    nodeTo->deleteEdgePointer(nodeFromNum);
    delete this;
}


