
#include <cstddef>
#include <algorithm>
#include <map>

#include "Node.h"
#include "Edge.h"



/**
 * Node counter
 */
size_t Node::count = 0;



size_t Node::getCount()
{
    return Node::count;
}



Node::Node(size_t id) :
    nodeId(id),
    edgeList(new EdgeList)
{
    count++;
}



Node::~Node()
{
    for(auto it = edgeList->begin(); it != edgeList->end(); ++it)
    {
        it->second->deleteEdge(nodeId);
    }
    delete edgeList;
    count--;
}



void Node::setId(size_t num)
{
    nodeId = num;
}



size_t Node::getId() const
{
    return this->nodeId;
}



/**
 * @brief Node::addEdge - add edge pointer in edge list
 * @param edge - edge pointed on this Node
 * @return true if pointer inserted
 */
bool Node::addEdge(Edge* edge)
{
    int nodeToNum = edge->nodeToNum(nodeId);
    return edgeList->insert( std::pair<int, Edge*>(nodeToNum, edge) ).second;
}



/**
 * @brief Node::getEdge - find existing edge from current node to second node.
 * @param nodeToNum - second node id.
 * @return pointer on Edge if exists, else null returned.
 */
const Edge* Node::getEdge(size_t nodeToNum) const
{
    auto edgeIterator = edgeList->find(nodeToNum);
    if (edgeIterator == edgeList->end())
    {
        return nullptr;
    }
    return edgeIterator->second;
}



/**
 * @brief Node::deleteEdge remove pointer from edge list on this edge
 * @param nodeToNum - second node number connected by edge
 */
void Node::deleteEdgePointer(size_t nodeToNum)
{
    edgeList->erase(nodeToNum);
}



const EdgeList* Node::getEdges() const
{
    return edgeList;
}



size_t Node::getEdgeCount() const
{
    return edgeList->size();
}



/**
 * @brief Node::getValueSortedEdges - Return sorted by edge weight list copy.
 * @return pointer on copy weight sorted edges vector
 */
WeightedEdges *Node::getValueSortedEdges()
{
    WeightedEdges* list = new WeightedEdges;
    float weight;
    for (auto it = edgeList->begin(); it != edgeList->end(); ++it)
    {
        weight = it->second->getValue();
        list->push_back( std::pair<float, Edge*>(weight, it->second) );
    }
    std::sort(list->begin(), list->end());
    return list;
}



