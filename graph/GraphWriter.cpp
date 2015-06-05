
#include <iostream>

#include "GraphWriter.h"
#include "Node.h"
#include "Edge.h"
#include "GraphBase.h"
#include "GraphReader.h"




GraphWriter::GraphWriter(const GraphBase &graph) :
    graph(&graph)
{ }



GraphWriter::~GraphWriter()
{ }



/**
 * @brief GraphWriter::openFile - open file on write and print log info if some
 * goes wrong
 * @param fileName - file name to write
 * @return file hadle if success or nullptr if error
 */
FILE* GraphWriter::openFile(const char *fileName)
{
    if (fileName == nullptr)
    {
        std::clog << "[!!!] Error: File name not set" << std::endl;
        return nullptr;
    }

    FILE* f = fopen(fileName, "w");
    if (f == nullptr)
    {
        std::clog << "[!!!] Error: Can't open file on write - " << fileName
                  << std::endl;
        return nullptr;
    }
    return f;
}



/**
 * @brief GraphWriter::writeEdges - Write in file graph format by
 * Node Node Value. Options can apply (PRINT_VALUE, PRINT_INFO).
 * If PRINT_VALUE false - global weight must be printed by PRINT_INFO in graph
 * otherwise weight will be lost.
 * @param fileName - file name
 * @param opttions - options to format out
 * @return true if file opened and graph writed
 */
bool GraphWriter::writeEdges(const char *fileName, cuint options)
{
    FILE* f = openFile(fileName);
    if (f == nullptr)
    {
        return false;
    }

    bool outInfo    = options & (unsigned) Option::PRINT_INFO;
    bool printValue = options & (unsigned) Option::PRINT_VALUE;

    GraphReader::Type typeId = printValue ? GraphReader::Type::NODE_NODE_VALUE :
                                            GraphReader::Type::NODE_NODE;

    const char* fileType = FileTypes<GraphReader::Type>::
                           typeName(typeId, GraphReader::types);
    fputs(fileType, f);
    fputc('\n', f);

    if (outInfo)
    {
        graph->printInfo(f);
    }

    unsigned nodeFromId, nodeToId, progress = 0;
    const EdgeList* edges;
    NodeMap* nodeList = graph->getNodeMap();

    startProcess(0, nodeList->size());

    for (auto it = nodeList->begin(); it != nodeList->end(); ++it, ++progress)
    {
        updateProgress(progress);
        if (isInterrupted())
        {
            fclose(f);
            return false;
        }
        nodeFromId = it->first;
        edges = it->second->getEdges();
        for (auto jt = edges->begin(); jt != edges->end(); ++jt)
        {
            nodeToId = jt->first;
            if (nodeToId < nodeFromId)
            {
                continue;
            }
            if (printValue)
            {
                fprintf(f, "%u %u %g\n", nodeFromId, nodeToId,
                        jt->second->getValue());
            }
            else
            {
                fprintf(f, "%u %u\n", nodeFromId, nodeToId);
            }
        }
    }
    fclose(f);
    completeProcess();
    return true;
}



/**
 * @brief GraphWriter::writeBracketsFlat - Create file in Bracket Style Graph.
 * Structure: Node1(Node2[EdgeValue] Node2[EdgeValue]...)
 * @param fileName - file name to write
 * @param opttions - options to format out
 */
bool GraphWriter::writeBracketsFlat(const char* fileName, cuint options)
{
    FILE* f = openFile(fileName);
    if (f == nullptr)
    {
        return false;
    }
    bool printInfo  = options & (unsigned) Option::PRINT_INFO;
    bool printValue = options & (unsigned) Option::PRINT_VALUE;

    GraphReader::Type typeId = printValue ?
                               GraphReader::Type::BRACKETS_FLAT_VALUE :
                               GraphReader::Type::BRACKETS_FLAT;

    const char* fileType = FileTypes<GraphReader::Type>::
                           typeName(typeId, GraphReader::types);
    fputs(fileType, f);
    fputc('\n', f);

    if (printInfo)
    {
        graph->printInfo(f);
    }

    const EdgeList* edges;
    Node* node;
    unsigned toNodeNum, progress = 0;
    float edgeValue;
    bool isFirst;
    NodeMap* nodeList = graph->getNodeMap();
    startProcess(0, nodeList->size() - 1);

    for (auto nodeIt = nodeList->begin(); nodeIt != nodeList->end();
         ++nodeIt, ++progress)
    {
        if (isInterrupted())
        {
            fclose(f);
            return false;
        }
        updateProgress(progress);
        node = nodeIt->second;
        edges = node->getEdges();
        fprintf(f, "%u(", nodeIt->first);
        isFirst = true;

        for (auto edgeIt = edges->begin(); edgeIt != edges->end(); ++edgeIt)
        {
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                fputs(" ", f);
            }
            toNodeNum = edgeIt->first;
            edgeValue = edgeIt->second->getValue();
            if (printValue)
            {
                fprintf(f, "%u[%g]", toNodeNum, edgeValue);
            }
            else
            {
                fprintf(f, "%u", toNodeNum);
            }
        }
        fputs(")\n", f);
    }
    fclose(f);
    completeProcess();
    return true;
}



/**
 * @brief GraphWriter::isNodeVisited - used in writeBrackets for check that
 * this node in visited list or not yet.
 * @param list - node deque stack
 * @param nodeId - searching node is
 * @return true if node in list.
 */
bool GraphWriter::isNodeVisited(const std::deque<unsigned>& list,
                                unsigned nodeId) const
{
    for(const auto &n : list)
    {
        if (n == nodeId)
            return true;
    }
    return false;
}



/**
 * @brief GraphWriter::writeBrackets - Write in file graph in bracket style.
 * It's write all connected nodes starting from startNodeId node.
 * All nodes writes using nesting. If node not found file not touched.
 * @param fileName    - file name for save graph
 * @param startNodeId - node id for start
 * @param pathLimit   - path nesting limit.
 * @param options     - output options
 * @return true if save, false if node not fonud or file can't to write.
 */
bool GraphWriter::writeBrackets(const char *fileName, cuint startNodeId,
                                cuint pathLimit, cuint options)
{
    const Node* nodeStart = graph->getNode(startNodeId);
    if (!nodeStart)
    {
        return false;
    }

    FILE* f = openFile(fileName);
    if (f == nullptr)
    {
        return false;
    }
    bool printInfo    = options & (unsigned) Option::PRINT_INFO;
    bool printValue   = options & (unsigned) Option::PRINT_VALUE;
    bool printIndents = options & (unsigned) Option::PRINT_INDENTS;

    GraphReader::Type typeId = printValue ? GraphReader::Type::BRACKETS_VALUE :
                                            GraphReader::Type::BRACKETS;

    const char* fileType = FileTypes<GraphReader::Type>::
                           typeName(typeId, GraphReader::types);
    fputs(fileType, f);
    fputc('\n', f);

    if (printInfo)
    {
        graph->printInfo(f);
    }

    fprintf(f, "%u(", startNodeId);
    if (printIndents)
    {
        fputc('\n', f);
    }

    bool isFirst = true;
    float weight;
    unsigned nodeId, process = 0;
    const Node* node;
    unsigned currentLimit = 1;

    std::deque <unsigned> nodeStack;
    std::deque <EdgeListPairIt> edgeStack;
    const EdgeList* edgeList = nodeStart->getEdges();
    auto edgeIt = edgeList->begin();
    auto edgeEnd = edgeList->end();

    startProcess(0, edgeList->size() - 1);

    nodeStack.push_front(startNodeId);
    edgeStack.push_front(EdgeListPairIt(edgeIt, edgeEnd));

    while(edgeStack.size())
    {
        if (isInterrupted())
        {
            fclose(f);
            return false;
        }
        edgeIt  = edgeStack.front().first;
        edgeEnd = edgeStack.front().second;
        if (edgeIt == edgeEnd)
        {
            --currentLimit;
            nodeStack.pop_front();
            edgeStack.pop_front();
            if (nodeStack.size() == 1)
            {
                ++process;
                updateProgress(process);
            }
            if (printIndents)
            {
                for(unsigned i = 0; i < currentLimit; ++i)
                {
                    fputc('\t', f);
                }
                fputs(")\n", f);
            }
            else
            {
                fputc(')', f);
            }
            continue;
        }
        nodeId = edgeIt->first;
        weight = edgeIt->second->getValue();
        node = edgeIt->second->nodeElem(nodeId);

        ++(edgeStack.front().first);

        if (isNodeVisited(nodeStack, nodeId))
        {
            continue;
        }
        if (printIndents)
        {
            for(unsigned i = 0; i < currentLimit; ++i)
            {
                fputc('\t', f);
            }
        }
        else if (!isFirst)
        {
            fputc(' ', f);
        }
        else {
            isFirst = false;
        }

        if (printValue)
        {
            fprintf(f, "%u[%g]", nodeId, weight);
        }
        else
        {
            fprintf(f, "%u", nodeId);
        }

        edgeList = node->getEdges();
        if (edgeList->size() && (!pathLimit || currentLimit < pathLimit))
        {
            edgeIt = edgeList->begin();
            edgeEnd = edgeList->end();
            bool isToVisit = false;
            for(; edgeIt != edgeEnd; ++edgeIt)
            {
                if (!isNodeVisited(nodeStack, edgeIt->first))
                {
                    isToVisit = true;
                    break;
                }
            }
            if (isToVisit)
            {
                nodeStack.push_front(nodeId);
                edgeStack.push_front( EdgeListPairIt(edgeIt, edgeEnd) );
                ++currentLimit;
                isFirst = true;
                fputc('(', f);
            }
        }
        if (printIndents)
        {
            fputc('\n', f);
        }

    }

    fclose(f);
    completeProcess();
    return true;
}


