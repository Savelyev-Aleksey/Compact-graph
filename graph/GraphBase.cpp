
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>

#include "GraphBase.h"
#include "Node.h"
#include "Edge.h"



GraphBase::GraphBase() :
    nodeList(new NodeMap),
    info(new InfoDeque),
    edgeSize(0)
{ }



GraphBase::~GraphBase()
{
    clear();
    delete nodeList;
    delete info;

    std::clog << "GraphBase was cleaned from destructor" << std::endl;

    logStatus();
}



/**
 * @brief GraphBase::clear - clear graph
 */
void GraphBase::clear()
{
    clearInfo();
    clearNodes();
}



bool GraphBase::isEmpty() const
{
    return nodeList->size() == 0;
}



size_t GraphBase::nodeCount() const
{
    return nodeList->size();
}



size_t GraphBase::edgeCount() const
{
    return edgeSize;
}



/**
 * @brief Graph::clearNodes - clear node list
 */
void GraphBase::clearNodes()
{
    for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
    {
        delete it->second;
    }
    nodeList->clear();
    edgeSize = 0;
}



/**
 * @brief GraphBase::clearInfo - clear info about graph
 */
void GraphBase::clearInfo()
{
    for (size_t i = 0; i < info->size(); ++i)
    {
        delete[] info->at(i).first;
        delete[] info->at(i).second;
    }
    info->clear();
}



NodeMap* GraphBase::getNodeMap() const
{
    return nodeList;
}



/**
 * @brief GraphBase::getNodeIds - Return a new set of node ids of graph.
 * @return set of node ids
 */
NodeIdSet* GraphBase::getNodeIds()
{
    std::set <size_t>* graphNodes = new std::set <size_t>;

    for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
    {
        graphNodes->insert(it->first);
    }
    return graphNodes;
}



/**
 * @brief GraphBase::getNodeDegreeStatistic - Return graph degree statistic.
 * @return map of Node degree and matched count.
 */
UlongMap* GraphBase::getNodeDegreeStatistic() const
{
    if (!nodeList->size())
    {
        return nullptr;
    }
    size_t degree;
    UlongMap* map = new UlongMap();
    for (auto it = nodeList->begin(), end = nodeList->end(); it != end; ++it)
    {
        degree = it->second->getEdgeCount();
        auto result = map->insert({degree, 1});
        if (!result.second)
        {
            ++(result.first->second);
        }
    }
    return map;
}



/**
 * @brief GraphBase::addInfo - Get string contain param=value and parse it.
 * Transform to upper case and trim.
 * @param str - string contain param=value
 */
void GraphBase::addInfo(const char *str)
{
    if (!str)
    {
        return;
    }
    const char* pos = strchr(str, '=');
    if (pos == nullptr)
    {
        return;
    }

    std::string name(str, pos - str);
    std::string value(pos + 1);

    // upper case
    std::transform(name.begin(), name.end(), name.begin(),
                   (int (*)(int))std::toupper);
    // trim
    name.erase(0, name.find_first_not_of(" \t") );
    name.erase(name.find_last_not_of(" \t") +1, name.size() );

    // upper case
    std::transform(value.begin(), value.end(), value.begin(),
                   (int (*)(int))std::toupper);
    // trim
    value.erase(0, value.find_first_not_of(" \r\n\t") );
    value.erase(value.find_last_not_of(" \r\n\t}") + 1, value.size() );

    addInfo(name.data(), value.data());
}



/**
 * @brief GraphBase::addInfo - Add to graph additional info
 * @param name  - param name
 * @param value - param value
 */
void GraphBase::addInfo(const char *name, const char *value)
{
    if (!name || !value)
    {
        return;
    }
    size_t l1 = strlen(name)  + 1;
    size_t l2 = strlen(value) + 1;
    char* newName  = new char[l1];
    char* newValue = new char[l2];
    strcpy(newName,  name);
    strcpy(newValue, value);
    info->push_back( std::pair<char*, char*>(newName, newValue) );
}



/**
 * @brief GraphBase::getInfo - returns value from info params.
 * null returned if info not found
 * @param name - name of param search
 * @return pointer on value of param or nullptr if not found
 */
const char* GraphBase::getInfo(const char *name) const
{
    for (size_t i = 0; i < info->size(); ++i)
    {
        if (!strcmp(name, info->at(i).first))
        {
            return info->at(i).second;
        }
    }
    return nullptr;
}



const InfoDeque* GraphBase::getAllInfo() const
{
    return info;
}



/**
 * @brief GraphBase::printInfo - Print info about graph in file format by
 * {param=value}
 * @param f - file pointer
 */
void GraphBase::printInfo(FILE *f) const
{
    for (unsigned i = 0; i < info->size(); ++i)
    {
        fprintf(f, "{%s=%s}\n", info->at(i).first, info->at(i).second);
    }
}



/**
 * @brief GraphBase::scanInfo - Read info from file about graph.
 * It's placed in top of file after file type.
 * @param f - file pointer
 */
void GraphBase::scanInfo(FILE *f)
{
    char buf[200], bracket;
    fpos_t position;
    while (true)
    {
        fgetpos(f, &position);
        bracket = fgetc(f);
        if (bracket != '{')
        {
            fsetpos(f, &position);
            return;
        }
        fgets(buf, 200, f);
        addInfo(buf);
    }
}



/**
 * @brief Graph::addEdge Insert new edge in graph.
 * New nodes creates if not exists.
 * @param nodeFromNum - Id from node
 * @param nodeToNum   - Id to node
 * @param value       - edge value
 */
bool GraphBase::addEdge(size_t nodeFromNum, size_t nodeToNum, float value)
{
    Node *fromNode, *toNode;

    // Exclude edge-loops
    if (nodeFromNum == nodeToNum)
    {
        return false;
    }
    // Get pointer on first and second node before create edge
    fromNode = getNodeOrCreate(nodeFromNum);
    toNode   = getNodeOrCreate(nodeToNum);

    return addEdge(fromNode, toNode, value);
}



/**
 * @brief Graph::addEdge - overloaded function. Add edge for two nodes
 * @param nodeFrom - first node
 * @param nodeTo   - second node
 * @param value    - edge value
 * @return true if edge added, false if already exists, or one of node not set.
 */
bool GraphBase::addEdge(Node* nodeFrom, Node* nodeTo, float value)
{
    if (nodeFrom == nullptr || nodeTo == nullptr)
    {
        return false;
    }
    // creating edge
    Edge* edge = new Edge(nodeFrom, nodeTo, value);
    if ( !(nodeFrom->addEdge(edge) && nodeTo->addEdge(edge)) )
    {
        // remove dublicate
        delete edge;
        return false;
    }
    ++edgeSize;
    return true;
}



/**
 * @brief Graph::addNode Add new node in graph.
 * Just inserted node have no one edges yet.
 * @param nodeNum - node id.
 * @return true - if inserted. false - if already exists.
 */
bool GraphBase::addNode(size_t nodeNum)
{
    Node* node = new Node(nodeNum);
    bool result;
    result = nodeList->insert({nodeNum, node}).second;
    if (!result)
    {
        delete node;
    }
    return result;
}



/**
 * @brief Graph::newNode - Create new node elem and add into graph.
 * @param nodeNum - node id
 * @return return pointer on new or existing node
 */
Node* GraphBase::newNode(size_t nodeNum)
{
    Node* node = new Node(nodeNum);
    auto result = nodeList->insert({nodeNum, node});
    if (!result.second)
    {
        delete node;
        return result.first->second;
    }
    return node;
}



/**
 * @brief Graph::getNodeOrCreate - Find existing node or create new.
 * @param nodeNum - node id to find
 * @return pointer on node
 */
Node *GraphBase::getNodeOrCreate(size_t nodeNum)
{
    Node* node;
    auto nodeIterator = nodeList->find(nodeNum);
    if (nodeIterator != nodeList->end())
    {
        node = nodeIterator->second;
    }
    else
    {
        node = new Node(nodeNum);
        nodeList->insert({nodeNum, node});
    }
    return node;
}



/**
 * @brief Graph::getNode - find existing node and return pointer.
 * Return null if node not exists.
 * @param nodeNum - node id
 * @return pointer on node or nullptr
 */
const Node* GraphBase::getNode(size_t nodeNum) const
{
    auto nodeIterator = nodeList->find(nodeNum);
    if (nodeIterator != nodeList->end())
    {
        return nodeIterator->second;
    }
    return nullptr;
}



/**
 * @brief Graph::getEdge - find edge beetwen first and second node.
 * @param nodeFromNum - first node id
 * @param nodeToNum - second node id
 * @return const edge pointer or nullptr
 */
const Edge* GraphBase::getEdge(size_t nodeFromNum, size_t nodeToNum) const
{
    const auto nodeIterator = nodeList->find(nodeFromNum);
    if (nodeIterator == nodeList->end())
    {
        return nullptr;
    }
    return nodeIterator->second->getEdge(nodeToNum);
}



/**
 * Write in log current node and edge status
 */
void GraphBase::logStatus()
{
    std::clog << std::endl << "Print info:" << std::endl
              << "\tNode count: \t" << nodeList->size() << std::endl
              << "\tEdge count: \t" << edgeSize << std::endl
              << std::endl;
}

