#include <vector>
#include <cstring>


#include "GraphBase.h"
#include "GraphGenerator.h"



/**
 * @brief GraphGenerator::GraphGenerator
 * @param graph - Object graph to save structure
 */
GraphGenerator::GraphGenerator(GraphBase& graph) :
    Worker(),
    graph(&graph)
{ }



GraphGenerator::~GraphGenerator()
{ }



/**
 * @brief GraphGenerator::number_capacity find number binary length
 * @param number - decimal number for seak length
 * @return bits number of binary length
 */
unsigned GraphGenerator::number_capacity(unsigned number)
{
    unsigned n = 1u, x = number;

    if (x == 0u) return 0u;

    if ((x >> 16u) == 0u) {n = n + 16u; x = x << 16u;}
    if ((x >> 24u) == 0u) {n = n + 8u;  x = x << 8u;}
    if ((x >> 28u) == 0u) {n = n + 4u;  x = x << 4u;}
    if ((x >> 30u) == 0u) {n = n + 2u;  x = x << 2u;}
    n = n - (x >> 31u);

    return 32u - n;
}



/**
 * @brief GraphShape::generateHypercube - Create new graph Hypercube structure.
 * Old graph data will be removed.
 * @param dimention - dimention of cube
 * @param weight    - weight of edges
 */
void GraphGenerator::generateHypercube(unsigned dimention, float weight)
{
    // Be shure graph is empty
    graph->clear();

    graph->addInfo("TYPE", "HYPERCUBE");
    char dimentionStr[4u], weightStr[16u];
    snprintf(dimentionStr, 4u, "%u", dimention);
    snprintf(weightStr, 16u, "%g", weight);

    graph->addInfo("DIMENTION", dimentionStr);
    graph->addInfo("WEIGHT", weightStr);

    // 2^dimention
    unsigned end = 1u << dimention;

    startProcess(0u, end - 2u);

    // prepare nodes
    for (unsigned i = 0u; i < end; ++i)
    {
        if (isInterrupted())
            return;

        graph->addNode(i);
    }

    NodeMap* nodeList = graph->getNodeMap();
    auto itPrevEnd = nodeList->end();
    --itPrevEnd;
    Node* j;

    unsigned current = 0u;
    for (auto i = nodeList->begin(); i != itPrevEnd; ++i, ++current)
    {
        updateProgress(current);
        unsigned mask, toId, fromId = i->first;
        for(unsigned char pow = 0; pow < dimention; ++pow)
        {
            if (isInterrupted())
            {
                graph->clear();
                return;
            }

            mask = 1u << pow;
            // if mask bit is zero - we can increase number
            // Only increase number, because less number nodes
            // already added edge in current node (by binary distance == 1)
            if (!(fromId & mask))
            {
                toId = fromId | mask;
                j = graph->getNode(toId);
                graph->addEdge(i->second, j, weight);
            }
        }
    }
    completeProcess();
}



/**
 * @brief GraphShape::generateTor - Generate new tor graph structure.
 * @param bigRadius - It's a circle rotetion radius (node count)
 * @param smallRadius - It's a form circle radius (node count)
 */
void GraphGenerator::generateTorus(unsigned bigRadius, unsigned smallRadius,
                                 float weight)
{
    // Be shure graph is empty
    graph->clear();

    if (bigRadius < 3 || smallRadius < 3)
        return;

    graph->addInfo("TYPE", "TORUS");
    char bigRadiusStr[6u], smallRadiusStr[6u], weightStr[16u];
    snprintf(bigRadiusStr, 6u, "%u", bigRadius);
    snprintf(smallRadiusStr, 6u, "%u", smallRadius);
    snprintf(weightStr, 16u, "%g", weight);
    graph->addInfo("BIG_RADIUS", bigRadiusStr);
    graph->addInfo("SMALL_RADIUS", smallRadiusStr);
    graph->addInfo("WEIGHT", weightStr);


    Node** prevList = nullptr;
    Node** currrentList = nullptr;
    Node** firstList = nullptr;

    startProcess(0u, smallRadius * bigRadius - 1u);

    unsigned id = 0u;
    // Move by circle rotation
    for (unsigned i = 0u; i < bigRadius; ++i)
    {
        currrentList = new Node*[smallRadius];
        // Create new circle on i angle
        for (unsigned j = 0u; j < smallRadius; ++j, ++id)
        {
            if(isInterrupted())
            {
                graph->clear();
                return;
            }

            updateProgress(id);

            currrentList[j] = graph->newNode(id);
            if (j > 0u)
            {
                graph->addEdge(currrentList[j - 1u], currrentList[j], weight);
            }
        }
        // connect first and last node to circle
        graph->addEdge(currrentList[0u], currrentList[smallRadius - 1u],weight);

        if (prevList == nullptr)
        {
            firstList = prevList = currrentList;
            continue;
        }
        // Connect i-1 angle circle to i circle
        for (unsigned j = 0u; j < smallRadius; ++j)
        {
            graph->addEdge(prevList[j], currrentList[j], weight);
        }
        // Don't delete first list
        if (i > 1)
        {
            delete[] prevList;
        }
        prevList = currrentList;
    }
    // Connect bigRadius angle circle to first circle
    for (unsigned j = 0u; j < smallRadius; ++j)
    {
        graph->addEdge(firstList[j], currrentList[j], weight);
    }
    delete[] firstList;
    delete[] currrentList;

    completeProcess();
}

