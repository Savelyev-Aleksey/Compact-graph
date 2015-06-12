
#include <cmath>
#include <vector>
#include <cstring>


#include "GraphBase.h"
#include "GraphGenerator.h"



/**
 * @brief GraphGenerator::GraphGenerator
 * @param graph - Object graph to save structure
 */
GraphGenerator::GraphGenerator(GraphBase& graph) :
    graph(&graph)
{ }



GraphGenerator::~GraphGenerator()
{ }



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
    char dimentionStr[4], weightStr[16];
    snprintf(dimentionStr, 4, "%hhu", dimention);
    snprintf(weightStr, 16, "%g", weight);

    graph->addInfo("DIMENTION", dimentionStr);
    graph->addInfo("WEIGHT", weightStr);

    auto end = (unsigned) pow(2, dimention);
    unsigned distance, onesCount;
    for (unsigned i = 0u; i < end; ++i)
    {
        graph->addNode(i);
    }
    NodeMap* nodeList = graph->getNodeMap();
    auto itEnd = nodeList->end();
    auto itPrevEnd = nodeList->end()--;

    for (auto i = nodeList->begin(); i != itPrevEnd; ++i)
    {
        auto j = i;
        for (++j; j != itEnd; ++j)
        {
            // Binary distance - count different bits
            distance = i->first ^ j->first;
            for (onesCount = 0u; onesCount < 2 && distance; distance >>= 1)
            {
                if (distance & 1)
                {
                    ++onesCount;
                }
            }
            if (onesCount == 1)
            {
                graph->addEdge(i->second, j->second, weight);
            }
        }
    }
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

    graph->addInfo("TYPE", "TORUS");
    char bigRadiusStr[6], smallRadiusStr[6], weightStr[16];
    snprintf(bigRadiusStr, 6, "%u", bigRadius);
    snprintf(smallRadiusStr, 6, "%u", smallRadius);
    snprintf(weightStr, 16, "%g", weight);
    graph->addInfo("BIG_RADIUS", bigRadiusStr);
    graph->addInfo("SMALL_RADIUS", smallRadiusStr);
    graph->addInfo("WEIGHT", weightStr);


    Node** prevList = nullptr;
    Node** currrentList = nullptr;
    Node** firstList = nullptr;

    unsigned id = 0u;
    // Move by circle rotation
    for (unsigned i = 0u; i < bigRadius; ++i)
    {
        currrentList = new Node*[smallRadius];
        // Create new circle on i angle
        for (unsigned j = 0u; j < smallRadius; ++j, ++id)
        {
            currrentList[j] = graph->newNode(id);
            if (j > 0u)
            {
                graph->addEdge(currrentList[j - 1], currrentList[j], weight);
            }
        }
        // connect first and last node to circle
        graph->addEdge(currrentList[0u], currrentList[smallRadius - 1], weight);

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
}

